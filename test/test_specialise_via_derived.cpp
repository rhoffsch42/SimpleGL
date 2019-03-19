#include <iostream>

class Base
{
public:
	virtual	bool	isFunction(const void* target) = 0;
			void	doStuff(const void* target) {
				if (this->isFunction(target))
					std::cout << "doing stuff with the target ( ͡° ͜ʖ ͡°) : " << (size_t)target << std::endl;
				else
					std::cout << "doing nothing with the target : " << (size_t)target << std::endl;
			}
};

class Specialised : public Base
{
	bool isFunction(const void* target) {
		if ((size_t)target % 10 > 5)
			return true;
		return false;
	}
};

int		main(void)
{
	char			*str = strdup("hehe");
	Specialised		*var1 = new Specialised();
	
	var1->doStuff(str);
	return (0);
}