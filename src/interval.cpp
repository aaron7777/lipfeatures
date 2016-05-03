#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class interval {
	public:
		double begin;
		double end;
		interval(double b, double e):begin(b),end(e) {}

};


int main(int argc, char** argv) {

	FILE *f;
	FILE *f1;
	double begin, end;
	char s1[10];
	char s2[10];
	int num;
	double qbegin, qend; // question begin & end time
	int i = 0;

	if (argc < 4) {
		printf("Call this program like this:\n");
		printf("./getinterval [describer file name] [question interval file name] [output file name]\n");
		printf("eg: ./getinterval ../../res/3des-pamela.txt ../../res/3ques.txt ../../res/3interval\n");
        return 0;
    }

	f = fopen(argv[1], "r");

	if(f == NULL) {
		printf("The describer file %s cannot be opened\n", argv[1]);
		return 0;
	}

	std::vector<interval*> silentIntvl;

	while(fscanf(f,"%lf %s %s %lf", &begin, s1, s2, &end) == 4)
	{
		//printf("%lf %lf\n", begin, end);
		if (!strcmp(s2, "silent")) {
			//printf("%lf %lf\n", begin, end);
			silentIntvl.push_back(new interval(begin, end));
		}

	}

	fclose(f);

	f = fopen(argv[2], "r");

	if(f == NULL) {
		printf("The interrogator file %s cannot be opened\n", argv[2]);
		return 0;
	}

	f1 = fopen(argv[3], "w");

	if(f1 == NULL) {
		printf("The output file %s cannot be opened\n", argv[3]);
		return 0;
	}

	while(fscanf(f, "%d %lf %lf", &num, &qbegin, &qend) == 3) {
		while(i < silentIntvl.size() && silentIntvl[i]->end <= qend) {
			if(silentIntvl[i]->end > qbegin && silentIntvl[i]->begin < qbegin)
				fprintf(f1, "%d %d %d\n", num, (int)(qbegin*14.925373), (int)(silentIntvl[i]->end*14.925373));
			else if(silentIntvl[i]->begin >= qbegin)
				fprintf(f1, "%d %d %d\n", num, (int)(silentIntvl[i]->begin*14.925373), (int)(silentIntvl[i]->end*14.925373));
			i++;
		}
	}

	fclose(f);
	fclose(f1);

}
