
#include "stdafx.h"
#include "MainObject.h"

//const int PLAYER_WIDTH = 60;
//const int PLAYER_HEIGHT = 64;


MainObject::MainObject()
{
  frame_ = 0;
  x_pos_ = 0;
  y_pos_ = 0;
  x_val_ = 0;
  y_val_ = 0;
  think_time_ = 0;
  width_frame_ = 0;
  height_frame_ = 0;
  status_ = -1;
  map_x_ = 0;
  map_y_ = 0;
  on_ground_ = false;
  input_type_.left_ = 0;
  input_type_.right_ = 0;
  input_type_.down_ = 0;
  input_type_.jump_ = 0;
  input_type_.up_ = 0;
  money_count_ = 0;
  status_ = WALK_NONE;
  bullet_count_ = 0;
  bullet_limit_ = 5;
  reload_time_ = 2000; 
  last_reload_time_ = 0;
  can_fire_ = true;
}

MainObject::~MainObject()
{
  Free();
}


SDL_Rect MainObject::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = rect_.x;
  rect.y = rect_.y;
  rect.w = rect_.w/NUM_MAIN_FRAME;
  rect.h = rect_.h;
  return rect;
}

bool MainObject::CanFireBullet()
{

    if (bullet_count_ >= bullet_limit_)
    {

        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_reload_time_ >= reload_time_)
        {

            bullet_count_ = 0;
            last_reload_time_ = current_time;
            return true;
        }
        else
        {

            return false;
        }
    }


    return true;
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    // Handle key down events
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            UpdateImagePlayer(screen);
            break;

        case SDLK_LEFT:
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            UpdateImagePlayer(screen);
            break;

        case SDLK_SPACE:
            input_type_.jump_ = 1;
            break;
        }
    }

    // Handle key up events
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            input_type_.right_ = 0;
            break;

        case SDLK_LEFT:
            input_type_.left_ = 0;
            break;
        }
    }

    // Handle mouse button down events
    else if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_LEFT)
        {
            if (CanFireBullet())
            {
                BulletObject* p_bullet = new BulletObject();
                p_bullet->LoadImg("img//player_bullet.png", screen);

#ifdef USE_AUDIO 
                Mix_PlayChannel(-1, bullet_sound[0], 0);
#endif

                if (status_ == WALK_LEFT)
                {
                    p_bullet->set_dir_bullet(BulletObject::DIR_LEFT);
                    p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_ * 0.22);
                }
                else
                {
                    p_bullet->set_dir_bullet(BulletObject::DIR_RIGHT);
                    p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_ * 0.22);
                }

                p_bullet->set_x_val(20);
                p_bullet->set_y_val(20);
                p_bullet->set_is_move(true);
                p_bullet_list_.push_back(p_bullet);
                bullet_count_++;
            }
        }

        // Optional: handle right mouse click here
        // else if (events.button.button == SDL_BUTTON_RIGHT)
        // {
        //     input_type_.jump_ = 1;
        // }
    }

    // Handle mouse button up events
    else if (events.type == SDL_MOUSEBUTTONUP)
    {
        if (events.button.button == SDL_BUTTON_LEFT)
        {
            // Do nothing when left mouse button is released
        }
        else
        {
            // Do nothing for other mouse buttons
        }
    }
}


void MainObject::HandleBullet(SDL_Renderer* des)
{
  for (int i = 0; i < p_bullet_list_.size(); i++)
  {
    BulletObject* p_bullet = p_bullet_list_.at(i);
    if (p_bullet != NULL)
    {
      if (p_bullet->get_is_move()) 
      {
        //if (bullet_dir_ == DIR_RIGHT)
        p_bullet->HandelMove(SCREEN_WIDTH, SCREEN_HEIGHT);
        //else
        //  p_bullet->HandleMoveRightToLeft();
        p_bullet->Render(des);
      }
      else
      {
        if (p_bullet != NULL)
        {
          p_bullet_list_.erase(p_bullet_list_.begin() + i);
          delete p_bullet;
          p_bullet = NULL;
          
        }
      }
    }
  }
}

void MainObject::RemoveBullet(const int& idx) 
{
  if (p_bullet_list_.size() > 0 && idx < p_bullet_list_.size())
  {
    BulletObject* p_bullet = p_bullet_list_.at(idx);
    p_bullet_list_.erase(p_bullet_list_.begin() + idx);

    if (p_bullet)
    {
      delete p_bullet;
      p_bullet = NULL;
    }
  }
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
  bool ret = BaseObject::LoadImg(path, screen);

  if (ret == true)
  {
    width_frame_ = rect_.w/NUM_MAIN_FRAME;
    height_frame_ = rect_.h;
  }

  return ret;
}

void MainObject::set_clips()
{
    // Clip the sprites
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i <= NUM_MAIN_FRAME ; ++i)
        {
            frame_clip_[i].x = width_frame_ * i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}


void MainObject::Show(SDL_Renderer* des)
{

  UpdateImagePlayer(des);

  if((input_type_.left_ == 1 ||
    input_type_.right_ == 1  ))
  {
    frame_++;
  }
  else
  {
    frame_ = 0;
  }

  if( frame_ >= 8 )
  {
    frame_ = 0;
  }

  if (think_time_ == 0)
  {
    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_- map_y_;

    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
    if (currentClip != NULL)
    {
      renderQuad.w = currentClip->w;
      renderQuad.h = currentClip->h;
    }

    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad );
  }
}

void MainObject::DoPlayer(Map& g_map)
{
  if (think_time_ == 0)
  {
    x_val_ = 0;

    y_val_ += GRAVITY_SPEED;

    if (y_val_ >= MAX_FALL_SPEED)
    {
      y_val_ = MAX_FALL_SPEED;
    }

    if (input_type_.left_ == 1)
    {
      x_val_ -= PLAYER_SPEED;
    }

    else if (input_type_.right_ == 1)
    {
      x_val_+= PLAYER_SPEED;
    }

    if (input_type_.jump_ == 1)
    {
      if (on_ground_ == true)
      {
        y_val_ = -PLAYER_HIGHT_VAL;
      }
      on_ground_ = false;
      input_type_.jump_ = 0;
    }

    CheckToMap(g_map);

    CenterEntityOnMap(g_map);
  }

  if (think_time_ > 0)
  {
      think_time_--;

      if (think_time_ == 0)
      {
          if (x_pos_ > 256)
          {
              x_pos_ -= 256;
          }
          else
              x_pos_ = 0;
          y_pos_ = 0;
          x_val_ = 0;
          y_val_ = 0;
          on_ground_ = false;
      }
  }
}

void MainObject::CenterEntityOnMap(Map& g_map)
{
  g_map.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

  if (g_map.start_x_ < 0)
  {
    g_map.start_x_ = 0;
  }

  else if (g_map.start_x_ + SCREEN_WIDTH >= g_map.max_x_)
  {
    g_map.start_x_= g_map.max_x_ - SCREEN_WIDTH;
  }

  g_map.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

  if (g_map.start_y_ < 0)
  {
    g_map.start_y_ = 0;
  }

  else if (g_map.start_y_+ SCREEN_HEIGHT >= g_map.max_y_)
  {
    g_map.start_y_ = g_map.max_y_ - SCREEN_HEIGHT;
  }
}

void MainObject::CheckToMap(Map& g_map)
{
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  //Check Horizontal
  //on_ground_ = false;
  int height_min =    height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;    //SDLCommonFunc::GetMin(height_frame_ TILE_SIZE);


  x1 = (x_pos_ + x_val_) / TILE_SIZE;
  x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

  y1 = (y_pos_) / TILE_SIZE;
  y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

  // Check x1, x2 with full width of map
  // Check y1, y2 with full heigth of map
  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
      if (x_val_ > 0) // when object is moving by right  ===>
      {
          int val1 = g_map.tile[y1][x2];
          int val2 = g_map.tile[y2][x2];

          if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == STATE_MONEY2 || val2 == STATE_MONEY2)
          {
              g_map.tile[y1][x2] = 0;
              g_map.tile[y2][x2] = 0;
              IncreaseMoney();
          }
          else
          {
              // Check current position of map. It is not blank_tile.
              if ((val1 != BLANK_TILE) || (val2 != BLANK_TILE))
              {
                  // Fixed post of object at current post of map.
                  // => Cannot moving when press button
                  x_pos_ = x2 * TILE_SIZE;
                  x_pos_ -= width_frame_ + 1;
                  x_val_ = 0; // cannot moving
              }
          }
    }
    else if (x_val_ < 0) // When moving by left    <====
    {
        int val1 = g_map.tile[y1][x1];
        int val2 = g_map.tile[y2][x1];
        if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == STATE_MONEY2 || val2 == STATE_MONEY2)
        {
            g_map.tile[y1][x1] = 0;
            g_map.tile[y2][x1] = 0;
            IncreaseMoney();
        }
        else
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y2][x1] != BLANK_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }
  }

  
  int width_min =  width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;//SDLCommonFunc::GetMin(width_frame_, TILE_SIZE);

  x1 = (x_pos_) / TILE_SIZE;
  x2 = (x_pos_ + width_min) / TILE_SIZE;

  y1 = (y_pos_ + y_val_) / TILE_SIZE;
  y2 = (y_pos_ + y_val_ + height_frame_ -1) / TILE_SIZE;

  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
      if (y_val_ > 0)
      {
          //Similar for vertical
          int val1 = g_map.tile[y2][x1];
          int val2 = g_map.tile[y2][x2];

          if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == STATE_MONEY2 || val2 == STATE_MONEY2)
          {
              g_map.tile[y2][x1] = 0;
              g_map.tile[y2][x2] = 0;
              IncreaseMoney();
          }
          else
          {
              if ((val1 != BLANK_TILE) || (val2 != BLANK_TILE))
              {
                  y_pos_ = y2 * TILE_SIZE;
                  y_pos_ -= height_frame_;

                  y_val_ = 0;

                  on_ground_ = true;
                  if (status_ == WALK_NONE)
                  {
                      //input_type_.right_ = 1;
                      status_ = WALK_RIGHT;
                  }
              }
          }
      }
      else if (y_val_ < 0)
      {
          int val1 = g_map.tile[y1][x1];
          int val2 = g_map.tile[y1][x2];

          if (val1 == STATE_MONEY || val2 == STATE_MONEY || val1 == STATE_MONEY2 || val2 == STATE_MONEY2)
          {
              g_map.tile[y1][x2] = 0;
              g_map.tile[y1][x2] = 0;
              IncreaseMoney();
          }
          else
          {
              if ((val1 != BLANK_TILE) || (val2 != BLANK_TILE))
              {
                  y_pos_ = (y1 + 1) * TILE_SIZE;

                  y_val_ = 0;
              }
          }
      }
  }

  //If there is not collision with map tile. 
  x_pos_ += x_val_;
  y_pos_ += y_val_;

  if (x_pos_ < 0)
  {
    x_pos_ = 0;
  }
  else if (x_pos_ + width_frame_ >= g_map.max_x_)
  {
    x_pos_ = g_map.max_x_ - width_frame_ - 1;
  }

  if (y_pos_ > g_map.max_y_)
  {
    think_time_ = 60;
    //number_of_think_time_++;

  }
 

}

void MainObject::IncreaseMoney()
{
    money_count_++;
    //Mix_Chunk* beep_sound = Mix_LoadWAV(kSoundBeep);
    //if (beep_sound != NULL)
    //    Mix_PlayChannel(-1, beep_sound, 0 );
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des)
{
    if (on_ground_ == true)  // jump = 0;
    {
        if (status_ == WALK_LEFT )
        {
            if (input_type_.up_ == 0)
            {
                LoadImg(g_name_main_left, des);
            }
            /*else if (input_type_.up_ == 1)
            {
                LoadImg("img//player_up_left.png", des);
            }*/

            /*if (input_type_.left_ == 1 && input_type_.up_ == 1)
            {
                LoadImg("img//player_cheo_left.png", des);
            }*/
        }
        else
        {
            if (input_type_.up_ == 0)
            {
                LoadImg(g_name_main_right, des);
            }
            /*else if (input_type_.up_ == 1)
            {
                LoadImg("img//player_up_right.png", des);
            }*/

            /*if (input_type_.right_ == 1 && input_type_.up_ == 1)
            {
                LoadImg("img//player_cheo_right.png", des);
            }*/
        }
    }
    else // Jump always is  1
    {
        if (status_ == WALK_LEFT)
        {
            if (input_type_.left_ == 1)
            {
                if (input_type_.up_ == 0)
                {
                    LoadImg("img//jum_left.png", des);
                    
                }
                /*else if (input_type_.up_ == 1)
                {
                    LoadImg("img//player_cheo_left.png", des);
                }*/
            }
            else
            {
                if (input_type_.up_ == 0)
                {
                    LoadImg("img//jum_left.png", des);
                }
                /*if (input_type_.up_ == 1)
                {
                    LoadImg("img//player_up_left.png", des);
                }*/
            }
        }
        else
        {
            if (input_type_.right_ == 1)
            {
                if (input_type_.up_ == 0)
                {
                   LoadImg("img//jum_right.png", des);
                   
                }
                /*else
                {
                    LoadImg("img//player_cheo_right.png", des);
                }*/
            }
            else
            {
                if (input_type_.up_ == 0)
                {
                    LoadImg("img//jum_right.png", des);
                }
                /*else if (input_type_.up_ == 1)
                {
                    LoadImg("img//player_up_right.png", des);
                }*/
            }
        }
    }

    set_clips();
}