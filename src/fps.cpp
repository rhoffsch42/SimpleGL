/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 17:18:39 by jfortin           #+#    #+#             */
/*   Updated: 2019/10/30 09:27:12 by rhoffsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fps.hpp"
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;


Fps::Fps() {
}

Fps::Fps(unsigned int fps_val) {
	assert(fps_val > 0); // TODO replace by macro
	this->fps = fps_val;
	this->tick = 1.0 / this->fps;
	this->last_time = glfwGetTime();
	this->current_time = this->last_time;
	this->ellapsed_time = 0.0;
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

double	Fps::getTick(void) const {
	return this->tick;
}

bool	Fps::wait_for_next_frame() {
	this->current_time = glfwGetTime();
	this->ellapsed_time = this->current_time - this->last_time;
	if (this->ellapsed_time >= this->tick)
	{
		this->last_time = this->current_time;
		return (true);
	}
	else
		return (false);
}

void	Fps::printFps() const {
	double	cent;
	double	fps;

	fps = 1.0 / this->ellapsed_time;
	cent = fps - double(int(fps));
	if (cent >= 0.5)
		fps += 1.0;
	cout << (float)this->current_time << "\t" << int(fps) << "fps" << endl;
}

void	printGlobalFps(void) {
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
	cout << (float)current_time << "\t" << int(fps) << "fps" << endl;
	last_time += ellapsed_time;
}
