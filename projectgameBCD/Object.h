#ifndef Object
#define Object
#include "CommonFunction.h"
class Object;
{
public:
	Object()/*ham constuctor*/;
	~Object()/*giai phong du lieu*/;
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y };
	SDL_Rect GetRect() const { return rect_; }
	SDL_Texture* GetObject()const { return p_object_; }
	bool LoadImg(std::string path, SDL_Renderer * screen);
	void Render(SDL_Renderer * des, const SDL_Rect* clip = NULL);

protected:
	SDL_Texture* p_object_;//Bien luu hinh anh(
	SDL_Rect rect_;//bien luu kich thuoc(voi struct SDL_Rect(x,y,w,h)



}
#endif
