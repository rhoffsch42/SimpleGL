#include "uipanel.hpp"
#include "simplegl.h"
#include "compiler_settings.h"

#define DEFAULT_SIZE 100

#ifdef SGL_DEBUG
#define SGL_UI_PANEL_DEBUG
#endif
#ifdef SGL_UI_PANEL_DEBUG 
#define D(x) std::cout << "[UIPanel] " << x
#define D_(x) x
#define D_SPACER "-- uipanel.cpp -------------------------------------------------\n"
#define D_SPACER_END "----------------------------------------------------------------\n"
#else 
#define D(x)
#define D_(x)
#define D_SPACER ""
#define D_SPACER_END ""
#endif

void	UIPanel::init() {
	this->isClickable = true;
	this->_texture = nullptr;

	this->_posX = 0;
	this->_posY = 0;
	this->_width = DEFAULT_SIZE;
	this->_height = DEFAULT_SIZE;
	this->_posX2 = this->_posX + this->_width;
	this->_posY2 = this->_posY + this->_height;
}

UIPanel::UIPanel() {
	glGenFramebuffers(1, &this->_fbo);
	this->init();
}

UIPanel::UIPanel(Texture * tex) {
	glGenFramebuffers(1, &this->_fbo);
	this->init();
	if (tex) {
		this->setTexture(tex);
		this->setSize(tex->getWidth(), tex->getHeight());
	}
}

UIPanel::UIPanel(const UIPanel & src) {
	*this = src;
}

UIPanel &	UIPanel::operator=(const UIPanel & src) {
	//D(__PRETTY_FUNCTION__ << std::endl);
	this->isClickable = src.isClickable;
	glGenFramebuffers(1, &this->_fbo);
	this->setTexture(src._texture);
	this->_posX = src._posX;
	this->_posY = src._posY;
	this->_width = src._width;
	this->_height = src._height;
	this->_posX2 = src._posX2;
	this->_posY2 = src._posY2;
	return (*this);
}


UIPanel::~UIPanel() {
	glDeleteFramebuffers(1, &this->_fbo);
}

bool		UIPanel::isOnPanel(int glX, int glY) const {
	return (glX >= this->_posX && glX <= this->_posX2 && glY >= this->_posY && glY <= this->_posY2);
}

//bind the texture to the framebuffer
void		UIPanel::setTexture(Texture * tex) {
	this->_texture = tex;
	if (tex) {
		// attach
		//D("UIPanel : attach : " << this->_fbo << " | " << this->_texture->getId() << std::endl);
		glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,	this->_texture->getId(), 0);// mipmap level: 0(base)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


void		UIPanel::setPos(int glX, int glY) {
	this->_posX = glX;
	this->_posY = glY;
	this->_width = this->_posX2 - this->_posX;
	this->_height = this->_posY2 - this->_posY;
}
void		UIPanel::setPos2(int glX, int glY) {
	this->_posX2 = glX;
	this->_posY2 = glY;
	this->_width = this->_posX2 - this->_posX;
	this->_height = this->_posY2 - this->_posY;
}
void		UIPanel::setSize(int width, int height) {
	this->_width = width;
	this->_height = height;
	this->_posX2 = this->_posX + this->_width;
	this->_posY2 = this->_posY + this->_height;
}

int			UIPanel::getWidth() const { return this->_width; }
int			UIPanel::getHeight() const { return this->_height; }
GLuint		UIPanel::getFbo() const { return this->_fbo; };
Texture *	UIPanel::getTexture() const { return this->_texture; };


UIImage::UIImage(Texture* tex) : UIPanel(tex) {}
UIImage::UIImage(const UIPanel& src) : UIPanel(src) {}

void	UIImage::action(int glX, int glY) {
	(void)glX;
	(void)glY;
}