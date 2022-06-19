# MappedStack

An implmenation of maps and stacks provides a no SQL like database interface.

_Uma implementação de mapas e pilhas que providencia uma interface semlhante a de uma base de dados não estructurada._

```
#include <iostream>
#include "MappedStack.hpp"

int main(){
	MappedStack<std::string_view, std::string> ms;
    
    	ms.insert("hello", "world");
    
    	std::cout << *ms.at("hello") << std::endl; // world
}
```

## Try it out / Experimente

> install cmake, git and and a c++ then run the following command on your terminal.

> instale cmake, git e um compilador c++ depois execute o comando a seguir no seu terminal. 

```
git clone https://github.com/ReneMuala/MappedStack.git && cd MappedStack && mkdir build && cd build && cmake .. && cmake --build .
