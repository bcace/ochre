#ifndef tree_h
#define tree_h

#include "control.h"
#include <map>



namespace pine {

	class CiTreeNode : std::map<std::string, CiTreeNode>
	{
	public:
		std::string text;
	};



	class CiTree : public Control
	{
	public:

	private:
		CiTreeNode root;
	};
}

#endif
