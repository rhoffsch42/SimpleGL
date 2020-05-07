/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 17:18:28 by jfortin           #+#    #+#             */
/*   Updated: 2019/10/25 18:03:15 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cassert>

#define FPS_FRAME_AVERAGE	15

class Fps
{
	public:
		Fps(unsigned int fps_val);
		Fps(Fps const &instance);
		// Fps &operator=(Fps const &rhs);
		~Fps();

		void	setFps(unsigned int fps_val);
		double	getTick(void) const;
		bool	wait_for_next_frame();
		int		getFps();

		static void	printGlobalFps(void);

	private:
		Fps();

		unsigned int	fps;
		double			tick;
		double			last_time;
		double			current_time;
		double			ellapsed_time;

		//only calculated if getFps is called
		int				_counter;
		double			_lastFps[FPS_FRAME_AVERAGE];
};
