
#ifndef DISEASE_TYPES
#define DISEASE_TYPES
#include <iostream>


#define DISEASE_RESERVE 24

namespace gerryfudd::types::disease {

		enum DiseaseColor { black, blue, red, yellow };

		class DiseaseStatus {
      int reserve;
      bool cured;
			public: 
				DiseaseStatus();
				int place(int);
				int treat(int);
				void cure(void);
				int get_reserve(void);
				bool is_cured(void);
        friend std::ostream& operator<<(std::ostream&, const DiseaseStatus&);
		};
}

#endif
