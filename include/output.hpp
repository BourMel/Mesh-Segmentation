/*
 foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47

reset             0    everything back to normal)
bold/bright       1    often a brighter shade of the same colour)
underline         4
inverse           7    swap foreground and background colours)
bold/bright off  21
underline off    24
inverse off      27
*/

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m" 
#define BLUE    "\033[0;34m" 
#define MAGENTA "\033[0;35m" 
#define CYAN    "\033[0;36m"
#define UL_ON   "\033[4m"
#define UL_OFF  "\033[24m"
#define RESET   "\033[0m" 