#include <iostream>
#include <iomanip>
#include <cmath>

/*
	https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
*/

float	toRadian(float degree) {
	return (degree * float(M_PI) / 180.0f);
}
float	toDegree(float radian) {
	return (radian * (180.0f / float(M_PI)));
}

bool	isEqual(float a, float b, int ulp) {
	std::cout << std::fixed << std::setprecision(20) << std::abs(a - b) << std::endl;

	return ((std::abs(a - b) <= std::numeric_limits<float>::epsilon() * std::abs(a + b) * ulp)
		|| (std::abs(a + b) < std::numeric_limits<float>::min()));
}

int		main(void)
{

	float	originalR = 2.18166f;
	float	originalD = 125.0f;
	float	r = toRadian(originalD);
	float	d = toDegree(originalR);

	std::cout << "min     " << std::fixed << std::setprecision(60) << std::numeric_limits<float>::min() << std::endl;
	std::cout << "epsilon " << std::fixed << std::setprecision(20) << std::numeric_limits<float>::epsilon() << std::endl;
	std::cout << std::endl;

	std::cout << "originalR : 2.18166f" << std::endl;
	std::cout << "effective : " << std::fixed << std::setprecision(20) << originalR << std::endl;
	std::cout << "originalD : 125.0f" << std::endl;
	std::cout << "effective : " << std::fixed << std::setprecision(20) << originalD << std::endl;
	std::cout << std::endl;
	std::cout << "effective 125.0f to rad   : " << std::fixed << std::setprecision(20) << r << std::endl;
	std::cout << "effective 2.18166f to deg : " << std::fixed << std::setprecision(20) << d << std::endl;

	int	ulp = 4;
	std::cout << std::endl;
	std::cout << "Degree Difference :" << std::endl;
	std::cout << (isEqual(originalD, d, ulp) ? "~=" : "!=") << "\n" << std::endl;
	std::cout << "Radian Difference :" << std::endl;
	std::cout << (isEqual(originalR, r, ulp) ? "~=" : "!=") << "\n" << std::endl;
	return (0);
}
