#ifndef color_h
#define color_h

#include <string>
#include <vector>

#define BYTE_TO_FLOAT	0.003921569



namespace pine {

	struct Color
	{
		unsigned char c[4];

		Color() {
			c[0] = c[1] = c[2] = 0; c[3] = 255;
		}

		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
			c[0] = r; c[1] = g; c[2] = b; c[3] = a;
		}

		void Apply();
		void Apply(unsigned char);

		double RedD() const		{ return c[0] * BYTE_TO_FLOAT; }
		double GreenD() const	{ return c[1] * BYTE_TO_FLOAT; }
		double BlueD() const	{ return c[2] * BYTE_TO_FLOAT; }
	};



	enum SchemeColor {

		scoBkgr = 0,
		scoPopupBkgr,
		scoFrgr,
		scoDisabledFrgr,

		scoSelectionBkgr,

		scoText,
		scoKeyword,
		scoSection,
		scoClass,
		scoComment,
		scoNumber,
		scoString,

		scoError,

		scoEnd
	};



	class Scheme : public std::vector<Color>
	{
	public:
		std::string name;
		unsigned id;

		Scheme(const std::string &);
	};



	class Schema : public std::vector<Scheme>
	{
		friend class Scheme;

	public:
		static void CreateSchema();
		static Scheme &SelectedScheme();
		static void SelectScheme(unsigned int);
		static void SelectScheme(const std::string &);
		static Color &GetColor(int);
		static void ApplyColor(int);
		static void ApplyColor(int, unsigned char);

		static Schema::iterator Begin();
		static Schema::iterator End();

		Schema();

	private:
		static Schema *ins;
		static unsigned availableId;
		unsigned int selScheme;
	};
}

#endif
