
#ifndef DISEASE_TYPES
#define DISEASE_TYPES
#include <iostream>
#include <string>

#define DISEASE_RESERVE 24

namespace gerryfudd::types::disease {

		enum DiseaseColor { black, blue, red, yellow, none };
		std::string name_of(DiseaseColor);

		struct DiseaseStatus {
      int reserve;
      bool cured;
			DiseaseStatus();
			friend std::ostream& operator<<(std::ostream&, const DiseaseStatus&);
		};
}

#endif
