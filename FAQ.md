### How does the prolog grammar understood by this program look like?
The following conventions are used to describe the grammar:

| Operator     | Description |
| ------------ | ----------- |
| [content]    | content is optional |
| *content     | 0,1 or multiple times content |
| content % ,  | describes a comma seperated list of content |
| A \| B       | either A or B |
| "word"       | word is to be taken literal |

And here is the grammar:
```text
query_code = atom % , "."
program_code = *(clause | comment)
comment = "%" text... \n
atom = functor | constant | built_in_pred
clause_head = functor | constant
clause_body = atom % ,
clause =  clause_head [ ":-" clause_body ] "." [comment]
constant = a-z *(a-zA-Z0-9_)
variable = A-Z *(a-zA-Z0-9_)
functor = a-z *(a-zA-Z0-9_) "(" prolog_element % , ")"
list = "[]" | "[" prolog_element % , ["|" variable | list]"]"
number = *0-9
prolog_element = functor | constant | variable | list | expression

expression = sum
sum = product % ("+" | "-")
product = power % ("*" | "//")
power = value [ "^" value]
value = number | variable | "(" expession ")"


built_in_pred = "==" | "\==" | "is"
```
