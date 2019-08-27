natSymb(o). %o ist ein natsymb
natSymb(s(A)) :- natSymb(A). % wenn A ein natsymb ist, muss auch s(A) eins sein
 
%I.A.
add(o, A, A).
 
%I.S.: quasi "umschichten". B wird abgebaut, A aufgebaut für jeden Schritt
add(s(A), B, C) :- natSymb(A), natSymb(B), natSymb(C), add(A, s(B), C).
 
%I.A.
mult(o, X, o).
%I.S.: siehe Übung 1
mult(s(A), B, C) :- mult(A, B, D), add(D, B, C).
 
% X mit o addiert muss wieder o sein, dann gilt X=0
%eqZero(X) :-  natSymb(X), add(o, X, o).
 
%o equals zero. always.
eqZero(o).
 
%wenn A ein natSymb ist, ist s(A) auch eins. Da es natSymb nur für A>0 gibt, ist diese
%Bedingung nur erfüllt, wenn A > 0
neqZero(s(X)) :-  natSymb(X).
 
less(o, s(_)).
less(s(X), s(s(Y))) :- less(X, s(Y)), natSymb(X), natSymb(Y).
 
square(X, Y) :- mult(X, X, Y).
 
exp(_, o, s(o)).
 
%y mal x miteinander multen x^y = B
exp(X, s(Y), B) :- exp(X, Y, A), mult(A, X, B), natSymb(X), natSymb(Y), natSymb(B).
 
log(X, Y, A) :- exp(X, A, Y).
 
 
% list(o, list(s(o), list(s(s(o)), nil))).
 
natList(nil).
natList(list(A, T)) :- natSymb(A), natList(T).
 
invList(list(o, nil)).
invList(list(s(A), list(A, B))) :- invList(list(A, B)).
 
% A ist head von Bs
head(list(A, Bs), A) :- natList(Bs).
tail(list(_, Bs), Bs) :- natList(Bs).
 
append(nil, As, As).
append(list(D, As), Bs, list(D, Cs)) :- append(As, Bs, Cs).
 
% Aufgabe 20
% Die leere Liste hat keine Elemente
anz(nil, o).
% wenn Xs N Elemente hat, dann hat Xs mit einem Element davor ein Element mehr
anz(list(_Y, Xs), s(N)) :- anz(Xs, N).
 
% Aufgabe 21
% baum: nib ist der leere baum
binbaum(nib).
% wenn Ab und Bb Teilbäume sind, dann kann der nächsthöhere Baum diese beiden enthalten
binbaum(knoten(_, Ab, Bb)) :- binbaum(Ab), binbaum(Bb).
 
% root(Xb, Y) :- Y ist der Wurzeleintrag des binärbaums Xb
root(binbaum(knoten(X, Ab, Bb)), X) :- binbaum(Ab), binbaum(Bb).
 
% left(Xb, Yb) :- Yb ist der linke Teilbaum von Xb.
left(binbaum(knoten(_X, Ab, Bb)), Ab) :- binbaum(Ab), binbaum(Bb).
 
% right(Xb, Yb) :- Yb ist der rechte Teilbaum von Xb
right(binbaum(knoten(_X, Ab, Bb)), Bb) :- binbaum(Ab), binbaum(Bb).
