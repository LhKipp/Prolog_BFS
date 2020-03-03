###How does the prolog grammar understood by this program look like?
The following conventions are used to describe the grammar:
    [content]       content is optional
    *content        0,1 or multiple times content
    content % ,     describes a comma seperated list of content
    A | B           either A or B
    "word"          word is to be taken literal
    
And here is the grammar:
```text
query_code = (functor | constant) % , "."
program_code = *(clause | comment)
comment = "%" text... \n
clause =  (functor | constant) [":-" "(" (functor | constant)%, ")"] "." [comment]
constant = a-z *(a-zA-Z0-9_)
variable = A-Z *(a-zA-Z0-9_)
functor = a-z *(a-zA-Z0-9_) "(" prolog_element % , ")"
list = "[]" | "[" prolog_element % , ["|" variable | list]"]"
prolog_element = functor | constant | variable | list
```
