#ifndef HIRSCH3D_TEXT_HPP
#define HIRSCH3D_TEXT_HPP
#include <string>
#include "Object.hpp"
namespace h3d
{
    
class Text: public Object {

public:

    void load(std::string text, std::string font, uint16_t fontsize);
    

private:



};



} // namespace h3d



#endif