#ifndef NUMBEREX_H
#define NUMBEREX_H

#include<string>

class NumberEx {
public:
    virtual ~NumberEx() = default;

    virtual void from_string(const std::string& str) = 0;
    virtual std::string to_string() const = 0;

};
#endif //NUMBEREX_H