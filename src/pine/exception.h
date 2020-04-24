#ifndef exception_h
#define exception_h

#include <exception>
#include <string>



namespace pine {

	class Exception : public std::exception
	{
	public:
		Exception(const std::string &_w) : w(_w) {}
		~Exception() throw() {}

		const char *what() const throw() { return w.c_str(); }

	private:
		std::string w;
	};
}

#endif
