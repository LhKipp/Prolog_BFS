% Author:
% Date: 14/04/2020

eltern(e1,k1).
eltern(e2,k1).
eltern(e1,k2).
eltern(e2,k2).
maennlich(e1).
weiblich(e2).
maennlich(k1).
weiblich(k2).

%durch Angabe von Fakten. braucht man für vater und mutter.
vater(X,Y) :- eltern(X,Y), maennlich(X).
% X ist vater von Y.
mutter(X,Y) :- eltern(X,Y), weiblich(X).
% X ist mutter von Y
sohn(X,Y) :- kind(X,Y), maennlich(X).

% X ist sohn von Y.
tochter(X,Y) :- kind(X,Y), weiblich(X).
% X ist tochter von Y.
bruder(X,Y) :- maennlich(X), ges(X,Y).
% X ist bruder von Y.
schwester(X,Y) :- weiblich(X), ges(X,Y).


kind(X,Y) :- eltern(Y,X).
% X ist kind von Y.

% X ist geschwister von Y.
%onkel(X,Y) :- maennlich(X), ges(X,R), eltern(R,Y).
% X ist Onkel von Y.
cousine(X,Y) :- weiblich(X), ges(M,R), eltern(M,X), eltern(R,Y).
% X ist cousine von Y.
grossvater(X,Y) :- vater(X,G), eltern(G,Y).
% X ist großvater von Y.

isNotSame(X,Y) :- X\==Y.

a(x(H,f)).
b(x(H,f)).
neq(X,Y) :- a(X), b(Y), X \== Y.
