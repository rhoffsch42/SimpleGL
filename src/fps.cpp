/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 17:18:39 by jfortin           #+#    #+#             */
/*   Updated: 2019/10/31 18:57:52 by rhoffsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fps.hpp"
#include <GLFW/glfw3.h>

#include <iostream>

#ifdef SGL_DEBUG
 #define SGL_FPS_DEBUG
#endif
#ifdef SGL_FPS_DEBUG 
 #define D(x) std::cout << "[Fps] " << x ;
 #define D_(x) x
 #define D_SPACER "-- fps.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif


Fps::Fps() {
}

Fps::Fps(unsigned int fps_val) {
	assert(fps_val > 0); // TODO replace by macro
	this->_fps = fps_val;
	this->_tick = 1.0 / this->_fps;
	this->_lastTime = glfwGetTime();
	this->_currentTime = this->_lastTime;
	this->_ellapsedTime = 0.0;

	this->_counter = 0;
	for (size_t i = 0; i < FPS_FRAME_AVERAGE; i++)
		this->_lastFps[i] = 0.0;
}

Fps::Fps(Fps const& instance) {
	(void)instance;
}

Fps::~Fps() {
}

// Fps &	Fps::operator=(Fps const &rhs) {
// }

void	Fps::setFps(unsigned int fps_val) {
	assert(fps_val > 0); // todo replace by macro
	this->_fps = fps_val;
	this->_tick = 1.0 / double(this->_fps);
	this->_maxTick = this->_tick;
}

double	Fps::getTick(void) const { return this->_tick; }
int		Fps::getMaxFps() const { return this->_fps; }


bool	Fps::wait_for_next_frame() {
	this->_currentTime = glfwGetTime();
	this->_ellapsedTime = this->_currentTime - this->_lastTime;
	if (this->_ellapsedTime >= this->_maxTick) {
		this->_lastTime = this->_currentTime;
		return (true);
	}
	else {
		return (false);
	}
}

int	Fps::getFps() {
	double	cent;
	double	fps;//if last frame time was constant

	fps = 1.0 / this->_ellapsedTime;
	cent = fps - double(int(fps));
	if (cent >= 0.5)
		fps += 1.0;

	//average fps of FPS_FRAME_AVERAGE last frames
	this->_lastFps[this->_counter] = fps;
	this->_counter++;
	if (this->_counter >= FPS_FRAME_AVERAGE)//we could use a modulo, but a time will come where it will overflow
		this->_counter = 0;
	fps = 0;
	for (size_t i = 0; i < FPS_FRAME_AVERAGE; i++)
		fps += this->_lastFps[i];
	fps /= FPS_FRAME_AVERAGE;

	this->_tick = 1.0 / fps;
	return fps;
	//D((float)this->_currentTime << "\t" << int(fps) << "fps" << endl)
}

void	Fps::printGlobalFps(void) {
	static double last_time = 0;
	static double ellapsed_time = 0;
	double	current_time;
	double	fps;
	double	cent;

	current_time = glfwGetTime();
	ellapsed_time = current_time - last_time;
	fps = 1.0 / ellapsed_time;
	cent = fps - double(int(fps));
	if (cent >= 0.5)
		fps += 1.0;
	D((float)_currentTime << "\t" << int(fps) << "fps" << std::endl);
	last_time += ellapsed_time;
}
