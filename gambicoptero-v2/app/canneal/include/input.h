/*
 * Define an input array. In order to get a new input array
 * you should run the canneal.pl script in $EPOS/app/canneal/input_generation/
 * and copy the output in canneal_input_epos.txt to here.
 * Author: Giovani Gracioli <giovani@lisha.ufsc.br>
 */

#ifndef INPUT_H
#define INPUT_H

#include <utility/string.h>

#define Y_SIZE 200

/*const int input_num_elements = 10; 
const int input_max_x = 5; 
const int input_max_y = 5;

const char input_array[input_num_elements][Y_SIZE] = 
{ 
"a 2 g a h d h END",
"b 2 c j g j a END" ,   
"c 2 h e c a f END" ,   
"d 1 j g b f j END" ,   
"e 1 i g c d c END" ,   
"f 1 a c j a g END" ,   
"g 2 d e b h j END" ,   
"h 1 f c e b c END" ,   
"i 1 e c i c h END" ,   
"j 2 i f h a d END" 
};*/

static const int input_num_elements = 50;
static const int input_max_x = 10;
static const int input_max_y = 10;

#define VECTOR_SIZE 100 // used in vector.h should be >= to input_num_elements
#define MAP_SIZE 100 // used in map.h should be >= to input_num_elements
#define MAX_STRING 20 // used in map.h


const char input_array[input_num_elements][Y_SIZE] = 
{ 
"a 2 ak ar m as w END",
"b 2 ai at s y g END",
"c 1 aa ai at m f END",
"d 2 al av ar m ao END",
"e 2 d aa av ap ar END",
"f 2 m x ag a ao END",
"g 1 u an h m s END",
"h 2 ap ax x y i END",
"i 1 av at aa ai al END",
"j 2 z d as ao y END",
"k 1 l m aj v an END",
"l 1 j aj o m m END",
"m 1 al f f s al END",
"n 2 g ap q j b END",
"o 1 u af ad i ap END",
"p 1 n z ar s av END",
"q 1 z w ax z aq END",
"r 2 q t j al q END",
"s 1 ae ar n k k END",
"t 2 c v ax n ao END",
"u 2 ad m s an ag END",
"v 1 ap ag d t y END",
"w 2 b ap o p au END",
"x 1 ai g ap w an END",
"y 1 p u d ai i END",
"z 1 av aj ax c t END",
"aa 2 ax n am ap ai END",
"ab 1 aq ak t an z END",
"ac 2 d an am al ad END",
"ad 1 c ak ac f al END",
"ae 1 y f ar ag g END",
"af 1 x aa h f aj END",
"ag 1 ah ao m at s END",
"ah 2 r ab ak m w END",
"ai 2 v q d w au END",
"aj 2 k c al ad ah END",
"ak 2 al m q v as END",
"al 2 f ag am l b END",
"am 2 at au j a am END",
"an 1 ao l an ax n END",
"ao 2 x c b an b END",
"ap 2 ah r v ai d END",
"aq 1 a ae as ab w END",
"ar 1 e as j ad l END",
"as 2 x ac s p ac END",
"at 1 x u as v c END",
"au 1 h o m k at END",
"av 2 d j i d p END",
"aw 2 af a d d ag END",
"ax 2 ag ae an ab o END",
};


class ifstream {
public:
    ifstream(const char *filename): _x(0), _y(0), _end(false), _end_line(false) { }
    
    bool is_open() { return true; }
    
    bool eof() { return _end; }
    
    bool operator>> (char *s) { 
        char name[10];
        int i = 0;
        
        if(_x == input_num_elements) {
            _end = true;
            return false;
        }
        
        if(_y == 0) _end_line = false;
        
        while(input_array[_x][_y] != ' ') {
            name[i] = input_array[_x][_y];
            if(strcmp(name, "END") == 0) {
                _y = 0;
                _x++;
                i++;
                
                if(_x == input_num_elements) {
                  _end = true;
                }
                _end_line = true;
                break;
            }
            i++;
            _y++;
        }
        
        if(!_end_line)
          _y++;
        
        name[i] = '\0';
        strcpy(s, name);
        return true;
    }
    
    bool operator>> (int &i) { 
        char name[10];
        if(_x == input_num_elements) {     
            _end = true;
            return false;
        }
                
        i = 0;
        
        while(input_array[_x][_y] != ' ') {
            name[i] = input_array[_x][_y];
            i++;
            _y++;
        }
        
        _y++;
        
        name[i] = '\0';
        i = atoi(name);
        
        return true;
    }
    
private:
    int _x, _y;
    bool _end;
    bool _end_line;
};

#endif
