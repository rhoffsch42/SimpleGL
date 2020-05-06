#pragma once

#include "texture.hpp"
#include "framebuffer.hpp"


class UIPanel {
public:
	UIPanel();
	UIPanel(Texture * tex);//can be null
	UIPanel(const UIPanel & src);
	UIPanel &	operator=(const UIPanel & src);
	~UIPanel();
	
			bool	isOnPanel(int glX, int glY) const;
	virtual	void	action(int glX, int glY) = 0;

	void		setTexture(Texture * tex);//attach it to fbo
	//undefined behaviors if positions are fucked up (neg pos, pos2 < pos, neg size)
	void		setPos(int glX, int glY);//modify bottom left corner, update size accordingly
	void		setPos2(int glX, int glY);//modify top right corner, update size accordingly
	void		setSize(int width, int height);//modify size, update top right corner accordingly

	int			getWidth() const;
	int			getHeight() const;
	GLuint		getFbo() const;
	Texture *	getTexture() const;

	bool		isClickable;
protected:
	GLuint		_fbo;
	Texture *	_texture;
	int			_posX;//do a vec2int ?
	int			_posY;
	int			_width;
	int			_height;
	int			_posX2;
	int			_posY2;

	void	init();
	friend class Renderer;
};

#include "obj3d.hpp"

class UIImage : public UIPanel {
public:
	UIImage(Texture* tex);
	UIImage(const UIPanel & src);
	void	action(int glX, int glY);
};

/* sample
class UIAnimation : public UIPanel {
public:
	UIAnimation(Texture * tex) : UIPanel(tex) {}
	void	action(int glX, int glY, HumanManager * manager) {
		
	}
};
*/