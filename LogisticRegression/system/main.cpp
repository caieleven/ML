#include "LogisticRegression.h"
using namespace std;

int main(int argc, char* argv[])
{
    LogisticRegression lr(argv[1]);
    lr.train();
    return 0;

}