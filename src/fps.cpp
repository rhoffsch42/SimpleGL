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
	this->fps = fps_val;
	this->tick = 1.0 / this->fps;
	this->last_time = glfwGetTime();
	this->current_time = this->last_time;
	this->ellapsed_time = 0.0;

	this->_counter = 0;
	for (size_t i = 0; i < FPS_FRAME_AVERAGE; i++)
		this->_lastFps[i] = 0.0;
}

Fps::Fps(Fps const &instance) {
	(void)instance;
}

Fps::~Fps() {
}

// Fps &	Fps::operator=(Fps const &rhs) {
// }

void	Fps::setFps(unsigned int fps_val) {
	assert(fps_val > 0); // TODO replace by macro
	this->fps = fps_val;
	this->tick = 1.0 / double(this->fps);
}

double	Fps::getTick(void) const {	return this->tick; }
int		Fps::getMaxFps() const { return this->fps; }


bool	Fps::wait_for_next_frame() {
	this->current_time = glfwGetTime();
	this->ellapsed_time = this->current_time - this->last_time;
	if (this->ellapsed_time >= this->tick) {
		this->last_time = this->current_time;
		return (true);
	} else {
		return (false);
	}
}

int	Fps::getFps() {
	double	cent;
	double	fps;//if last frame time was constant

	fps = 1.0 / this->ellapsed_time;
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

	return fps;
	//D((float)this->current_time << "\t" << int(fps) << "fps" << endl)
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
	D((float)current_time << "\t" << int(fps) << "fps" << std::endl)
	last_time += ellapsed_time;
}
