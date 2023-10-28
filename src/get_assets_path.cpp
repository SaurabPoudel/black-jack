#include <string>
using namespace std;

string getAssetPath(const string &relativePath) {
  return string(__FILE__).substr(0, std::string(__FILE__).rfind("/")) +
         "/../assets/" + relativePath;
}