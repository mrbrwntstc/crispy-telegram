#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"

// game-related state data
// -----------------------
SpriteRenderer *renderer;
GameObject *player;
BallObject *ball;

bool check_collision(GameObject &one, GameObject &two);
collision check_collision(BallObject &one, GameObject &two);
Direction vector_direction(glm::vec2 target);

Game::Game(unsigned int width, unsigned int height)
    : state(GameState::Active),
      keys(),
      width(width),
      height(height)
{
}

Game::~Game()
{
  delete renderer;
  delete player;
  delete ball;
}

void
Game::init()
{
  ResourceManager::load_shader("../assets/shaders/sprite.vs", "../assets/shaders/sprite.fs", nullptr, "sprite");
  // configure shaders
  // -----------------
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
  ResourceManager::get_shader("sprite").use().set_integer("image", 0);
  ResourceManager::get_shader("sprite").set_matrix4("projection", projection);
  // set render-specific controls
  // ----------------------------
  renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));
  // load textures
  // -------------
  ResourceManager::load_texture("../assets/textures/awesomeface.png", true, "face");
  ResourceManager::load_texture("../assets/textures/background.jpg", false, "background");
  ResourceManager::load_texture("../assets/textures/block.png", false, "block");
  ResourceManager::load_texture("../assets/textures/block_solid.png", false, "block_solid");
  ResourceManager::load_texture("../assets/textures/paddle.png", true, "paddle");
  // load levels
  // -----------
  GameLevel one;
  one.load("../assets/levels/one.lvl", this->width, this->height / 2);
  GameLevel two;
  two.load("../assets/levels/two.lvl", this->width, this->height / 2);
  GameLevel three;
  three.load("../assets/levels/three.lvl", this->width, this->height / 2);
  GameLevel four;
  four.load("../assets/levels/four.lvl", this->width, this->height / 2);

  this->levels.push_back(one);
  this->levels.push_back(two);
  this->levels.push_back(three);
  this->levels.push_back(four);
  this->level = 0;

  // configure game objects
  // ----------------------
  // player
  // ------
  glm::vec2 player_position = glm::vec2(this->width / 2.0f - player_size.x / 2.0f, this->height - player_size.y);
  player = new GameObject(player_position, player_size, ResourceManager::get_texture("paddle"));
  // ball
  // ----
  glm::vec2 ball_position = player_position + glm::vec2(player_size.x / 2.0f - ball_radius, -ball_radius * 2.0f);
  ball = new BallObject(ball_position, ball_radius, initial_ball_velocity, ResourceManager::get_texture("face"));
}

void
Game::update(float delta)
{
  ball->move(delta, this->width);
  this->do_collisions();
}

void
Game::process_input(float delta)
{
  if(this->state == GameState::Active) {
    float velocity = player_velocity * delta;
    // move player
    if(this->keys[GLFW_KEY_A]) {
      if(player->position.x >= 0.0f) {
        player->position.x -= velocity;
        if(ball->stuck) {
          ball->position.x -= velocity;
        }
      }
    }
    if(this->keys[GLFW_KEY_D]) {
      if(player->position.x <= this->width - player->size.x) {
        player->position.x += velocity;
        if(ball->stuck) {
          ball->position.x += velocity;
        }
      }
    }
    if(keys[GLFW_KEY_SPACE]) {
      ball->stuck = false;
    }
  }
}

void
Game::render()
{
  if(this->state == GameState::Active) {
    // draw background
    renderer->draw_sprite(ResourceManager::get_texture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
    // draw level
    this->levels[this->level].draw(*renderer);
    // draw player
    player->draw(*renderer);
    // draw ball
    ball->draw(*renderer);
  }
}

void
Game::do_collisions()
{
  for(GameObject &box : levels[level].bricks) {
    if(!box.destroyed) {
      collision collision = check_collision(*ball, box);
      if(std::get<0>(collision)) {
        // destroy block if not solid
        if(!box.is_solid) {
          box.destroyed = true;
        }
        // collision resolution
        Direction direction = std::get<1>(collision);
        glm::vec2 difference_vector = std::get<2>(collision);
        if(direction == Direction::Left || direction == Direction::Right) {
          // reverse horizontal velocity
          ball->velocity.x = -ball->velocity.x;
          // relocate
          float penetration = ball->radius - std::abs(difference_vector.x);
          if(direction == Direction::Left) {
            ball->position.x += penetration; // right
          } else {
            ball->position.x -= penetration; // left
          }
        } else {
          // reverse vertical velocity
          ball->velocity.y = -ball->velocity.y;
          // relocate
          float penetration = ball->radius - std::abs(difference_vector.y);
          if(direction == Direction::Up) {
            ball->position.y -= penetration;
          } else {
            ball->position.y += penetration;
          }
        }
      }
    }
  }

  // check collision for player pad (unless stuck)
  collision result = check_collision(*ball, *player);
  if(!ball->stuck && std::get<0>(result)) {
    // check whether it hit the board...
    float center_board = player->position.x + player->size.x / 2.0f;
    float distance = (ball->position.x + ball->radius) - center_board;
    float percentage = distance / (player->size.x / 2.0f);
    // .. then change velocity based on where it hit the board
    float strength = 2.0f;
    glm::vec2 old_velocity = ball->velocity;
    ball->velocity.x = initial_ball_velocity.x * percentage * strength;
    ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);
    // fix sticky paddle
    ball->velocity.y = -1.0f * abs(ball->velocity.y);
  }
}

// aabb-aabb collision
bool
check_collision(GameObject &one, GameObject &two)
{
  bool x_axis_collision = (one.position.x + one.size.x >= two.position.x) && (two.position.x + two.size.x >= one.position.x);
  bool y_axis_collision = (one.position.y + one.size.y >= two.position.y) && (two.position.y + two.size.y >= one.position.y);

  return x_axis_collision && y_axis_collision;
}

// circle-aabb collision
collision
check_collision(BallObject &one, GameObject &two)
{
  // center point of circle
  glm::vec2 ball_center(one.position + one.radius);
  // calculate AABB info (center + half-extents)
  glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
  glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);
  // get difference vector between both centers
  glm::vec2 difference_vector = ball_center - aabb_center;
  glm::vec2 clamp_vector = glm::clamp(difference_vector, -aabb_half_extents, aabb_half_extents);
  // add clamped value to AABB_center and get the value closest to circle
  glm::vec2 closest_point_on_brick_to_ball = aabb_center + clamp_vector;
  // vector between center circle and closest point aabb
  difference_vector = closest_point_on_brick_to_ball - ball_center;

  if(glm::length(difference_vector) < one.radius) {
    return std::make_tuple(true, vector_direction(difference_vector), difference_vector);
  } else {
    return std::make_tuple(false, Direction::Up, glm::vec2(0.0f, 0.0f));
  }
}

Direction
vector_direction(glm::vec2 target)
{
  glm::vec2 compass[] = {
    glm::vec2(0.0f, 1.0f),  // up
    glm::vec2(1.0f, 0.0f),  // right
    glm::vec2(0.0f, -1.0f), // down
    glm::vec2(-1.0f, 0.0f)  // left
  };

  float max = 0.0f;
  unsigned int best_match = -1;
  for(unsigned int i = 0; i < 4; i++) {
    float dot_product = glm::dot(glm::normalize(target), compass[i]);
    if(dot_product > max) {
      max = dot_product;
      best_match = i;
    }
  }

  return (Direction)best_match;
}