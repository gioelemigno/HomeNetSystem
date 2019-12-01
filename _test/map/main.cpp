#include <map>
#include <string>

std::map<std::string, int> map;
std::map<std::string, int>::iterator it;
int main(){
    map.insert(std::pair<std::string, int>("light_0", 4));
    map.insert(std::pair<std::string, int>("temp_0", 329));
    map.insert(std::pair<std::string, int>("socket_0", 549));

    it=map.find("light_0");
    printf("%s\n", it->first.c_str());
    printf("%d\n", it->second);

    it=map.find("kk");
    if(it==map.end()) printf("non found \n");
}