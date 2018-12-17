#include <Keypad.h>
#include "U8glib.h"//引用U8G头文件
#include"MsTimer2.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//设置设备名称：I2C-SSD1306-128*64(OLED)
/***********************************************/
#define UP 5//定义方向键上为Arduino上的D5号引脚
#define DOWN 4//定义方向键上为Arduino上的D4号引脚
#define LEFT 3//定义方向键上为Arduino上的D3号引脚
#define RIGHT 2//定义方向键上为Arduino上的D2号引脚
#define A 8//定义方向键上为Arduino上的D8号引脚
/***********************************************/
int box_x = 0;//定义变量
int box_y = 0;//定义变量
int box_x_length = 126;//x方向42  0-41
int box_y_length = 51;//y方向16  0-15
int snake_max_length = 100; //蛇身体最大长度
int snake_x[100];//蛇身x坐标
int snake_y[100];//蛇身y坐标
int snake_body_width = 3; //蛇身方块宽度（正方形）
int food_x;//食物位置坐标x
int food_y;//食物位置坐标y
int snake_length = 3; //定义初始化蛇身长度
unsigned int game_speed;//设置游戏速度
int notPause = 1;
int key = 0;
/***********************************************/
const byte ROWS = 2;
const byte COLS = 2;

char hexaKeys[ROWS][COLS] = {
  {'1', '3'},
  {'2', '4'},
};
byte rowPins[ROWS] = {5, 4};
byte colPins[COLS] = {3, 2};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
/***********************************************/
void setup(void)
{
  pinMode(A, INPUT_PULLUP);//定义按键A引脚状态
  MsTimer2::set(0, read_key);//定时器中断按键的扫描
  MsTimer2::start();
  welcome();//欢迎界面
  delay(2000);
  choose_game();//选择界面
}
/***********************************************/
void welcome()
{
  u8g.firstPage();//第一页显示
  do
  {
    u8g.setFont(u8g_font_gdr14r);//坐标函数
    u8g.setPrintPos(0, 24);
    u8g.print("Snake Game");

    u8g.setPrintPos(0, 40);
    u8g.setFont(u8g_font_6x12);
    u8g.print("Soochow University");

    u8g.setPrintPos(0, 56);
    u8g.setFont(u8g_font_6x12);
    u8g.print("YZune | Qin | Zhang");
  } while (u8g.nextPage());
}
/***********************************************/
int choose_game()//对选择界面进行定义
{
  int flag = 1;
  int temp = 1;
  while (flag)
  {
    if (key == UP)
    {
      u8g.firstPage();
      do
      {
        u8g.setFont(u8g_font_9x18);
        u8g.setPrintPos(0, 20);
        u8g.print("Start  <");
        u8g.setPrintPos(0, 40);
        u8g.print("Settings");
      } while (u8g.nextPage());
      temp = 1;
    }
    if (key == DOWN)
    {
      u8g.firstPage();
      do
      {
        u8g.setFont(u8g_font_9x18);
        u8g.setPrintPos(0, 20);
        u8g.print("Start");
        u8g.setPrintPos(0, 40);
        u8g.print("Settings  <");
      } while (u8g.nextPage());
      temp = 2;
    }
    if (key == A)
    {
      return temp;
    }
  }
}
/***********************************************/
void read_key()//对按键进行定义
{
  char customKey = customKeypad.getKey();
  if (customKey == '3') {
    key = UP;
  }
  else if (customKey == '2') {
    key = DOWN;
  }
  else if (customKey == '4') {
    key = LEFT;
  }
  else if (customKey == '1') {
    key = RIGHT;
  }
  else if (digitalRead(A) == LOW) {
    key = A;
  }
}
/***********************************************/
void game_over()//对游戏结束进行定义
{
  key = 0;
  u8g.firstPage();
  do
  {
    u8g.setFont(u8g_font_gdr14r);
    u8g.setPrintPos(0, 16);
    u8g.print("GAME OVER!");

    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(0, 40);
    u8g.print("Your Score:");

    u8g.setFont(u8g_font_9x18);
    u8g.setPrintPos(0, 56);
    u8g.print((snake_length - 3) * 5);
  } while (u8g.nextPage());
  snake_length = 3;
  snake_x[0] = 15; snake_y[0] = 15;//snake起始坐标
  snake_x[1] = snake_x[0]  - 1; snake_y[1] = snake_y[0];//snake起始坐标
  snake_x[2] = snake_x[1]  - 1; snake_y[2] = snake_y[1];//snake起始坐标
}

void snake()//对贪吃蛇参数进行定义
{
  int flag = 1; //标志
  snake_x[0] = 15; snake_y[0] = 15;//snake起始坐标
  snake_x[1] = snake_x[0]  - 1; snake_y[1] = snake_y[0];//snake起始坐标
  snake_x[2] = snake_x[1]  - 1; snake_y[2] = snake_y[1];//snake起始坐标
  int snake_dir = RIGHT; //初始方向 right
  int food_flag = 1;
  food();
  while (flag)
  {
    snake_frame(game_speed);
    delay(game_speed);
    switch (key)
    {
      case UP:
        notPause = 1;
        if (snake_dir != DOWN)
        {
          snake_dir = UP;
        }
        break;

      case DOWN:
        notPause = 1;
        if (snake_dir != UP)
        {
          snake_dir = DOWN;
        }
        break;

      case LEFT:
        notPause = 1;
        if (snake_dir != RIGHT)
        {
          snake_dir = LEFT;
        }
        break;

      case RIGHT:
        notPause = 1;
        if (snake_dir != LEFT)
        {
          snake_dir = RIGHT;
        }
        break;

      case A:
        notPause = 0;
        break;

      default: break;
    }
    if (snake_eat_food(snake_dir) == 1)
    {
      food();
      if (snake_length < 10) {
        game_speed = 30;
      }
      if (snake_length < 30 && snake_length >= 10) {
        game_speed = 20;
      }
      if (snake_length < 50 && snake_length >= 30) {
        game_speed = 10;
      }
      if (snake_length >= 50) {
        game_speed = 0;
      }
    }
    else
    {
      if (snake_knock_wall(snake_dir == 1))
      {
        game_over();
        flag = 0;
      }
      else
      {
        if (snake_eat_body(snake_dir) == 1)
        {
          game_over();
          flag = 0;
        }
        else
        {
          change_con(snake_dir);
        }
      }
    }
  }
}

void snake_frame(int s)
{
  u8g.firstPage();
  do
  {
    u8g.drawFrame(box_x, box_y, box_x_length, box_y_length);
    u8g.setFont(u8g_font_5x8);
    u8g.setPrintPos(0, box_y_length + 6);
    u8g.print("Score");
    u8g.setPrintPos(32, box_y_length + 6);
    u8g.print((snake_length - 3) * 5);
    //    u8g.setPrintPos(box_x_length + 1, 46);
    //    u8g.print("Speed");
    //    u8g.setPrintPos(box_x_length + 1, 56);
    //    u8g.print(30 - s);
    u8g.drawFrame(food_x * snake_body_width + 1, food_y * snake_body_width + 1, snake_body_width, snake_body_width); //显示食物
    for (int i = 0; i < snake_length; i++)//显示snake
    {
      if (i == 0)
      {
        u8g.drawBox(snake_x[i]*snake_body_width + 1, snake_y[i]*snake_body_width + 1, snake_body_width, snake_body_width);
      }
      else
      {
        u8g.drawFrame(snake_x[i]*snake_body_width + 1, snake_y[i]*snake_body_width + 1, snake_body_width, snake_body_width);
      }
    }
  } while (u8g.nextPage());
}

void food()
{
  int flag = 1;
  while (flag)
  {
    food_x = random(0, (box_x_length - 2) / 3);
    food_y = random(0, (box_y_length - 2) / 3);
    for (int i = 0; i < snake_length; i++)
    {
      if ((food_x == snake_x[i]) && (food_y == snake_y[i]))
      {
        break;
      }
      flag = 0;
    }
  }
}

void change_con(int DIR)//移动坐标函数
{
  if (notPause) {
    int temp_x[snake_length + 2];
    int temp_y[snake_length + 2];
    for (int i = 0; i < snake_length - 1; i++) //将旧坐标数据存放到temp数组  最后一位数据无效
    {
      temp_x[i] = snake_x[i];
      temp_y[i] = snake_y[i];
    }
    switch (DIR)
    {
      case RIGHT: {
          snake_x[0] += 1;
          break;
        }
      case LEFT: {
          snake_x[0] -= 1;
          break;
        }
      case UP: {
          snake_y[0] -= 1;
          break;
        }
      case DOWN: {
          snake_y[0] += 1;
          break;
        }
    }
    for (int i = 1; i < snake_length; i++)
    {
      snake_x[i] = temp_x[i - 1];
      snake_y[i] = temp_y[i - 1];
    }
  }
}

void change_pos()//吃到东西后移动函数
{
  int temp_x[snake_length + 2];
  int temp_y[snake_length + 2];
  for (int i = 0; i < snake_length - 1; i++) //将旧坐标数据存放到temp数组  最后一位数据无效
  {
    temp_x[i] = snake_x[i];
    temp_y[i] = snake_y[i];
  }
  snake_x[0] = food_x;
  snake_y[0] = food_y;
  for (int i = 1; i < snake_length; i++)
  {
    snake_x[i] = temp_x[i - 1];
    snake_y[i] = temp_y[i - 1];
  }
}
//判断是否吃到食物 0-没有吃到 1-吃到 2-无效
int snake_eat_food(int dir)
{
  int x_temp = snake_x[0];
  int y_temp = snake_y[0];
  switch (dir)
  {
    case UP : y_temp -= 1; break;
    case DOWN : y_temp += 1; break;
    case LEFT : x_temp -= 1; break;
    case RIGHT : x_temp += 1; break;
  }
  if ((x_temp == food_x) && (y_temp == food_y))
  {
    snake_length += 1;
    change_pos();
    return 1;
  }
  else
  {
    return 0;
  }
}
//判断是否撞墙 0-没有撞到 1-撞到 2-无效
int snake_knock_wall(int dir)
{
  int x_temp = snake_x[0];
  int y_temp = snake_y[0];
  switch (dir)
  {
    case UP : y_temp -= 1; break;
    case DOWN : y_temp += 1; break;
    case LEFT : x_temp -= 1; break;
    case RIGHT : x_temp += 1; break;
  }
  if (x_temp < 0 || x_temp > 40 || y_temp < 0 || y_temp > 15)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
//判断是否吃到自己 0-没有吃到 1-吃到 2-无效
int snake_eat_body(int dir)
{
  int x_temp = snake_x[0];
  int y_temp = snake_y[0];
  switch (dir)
  {
    case UP : y_temp -= 1; break;
    case DOWN : y_temp += 1; break;
    case LEFT : x_temp -= 1; break;
    case RIGHT : x_temp += 1; break;
  }
  for (int i = 1; i < snake_length; i++)
  {
    if ((snake_x[0] == snake_x[i]) && (snake_y[0] == snake_y[i]))
    {
      return 1;
    }
  }
  return 0;
}
/***********************************************/
void settings()
{
  int flag = 1;
  while (flag)
  {
    
  }
}
/***********************************************/
void loop(void)//主循环函数
{
  switch (choose_game())
  {
    case 1: snake(); break;
    case 2: settings(); break;
    default: break;
  }
}
