natSymb(o).
natSymb(s(X)) :- natSymb(X).

% aus Graph
kante(a,b).
kante(a,c).
kante(b,c). 
kante(b,d). 
kante(b,e). 
kante(c,d). 
kante(e,c). 
kante(e,d).
% ende aus Graph

% extra
kante(e,f).


% IA: es gibt eine weg von X zu sich selbst
weg(X, X).

% IS: wenn es eine Kante von X zu einem imaginären U gibt (ein Schritt)
% dann gibt einen weg (mehrere Kanten) von U nach Y
weg(X, Y) :- kante(X, U), weg(U, Y). 

% IA: der weg von X zu sich selbst ist o lang
wegLaenge(X, X, o).

% IS: baut auf der Idee von weg() auf, außer dass Laenge als Teil der Relation
% mitgezogen wird. Wenn der Teilweg "laenge" lang ist, dann ist der Weg im naechsten
% Schritt (+1 Kante) s(Laenge) lang.
wegLaenge(X, Y, s(Laenge)) :- kante(X, U), wegLaenge(U, Y, Laenge), natSymb(Laenge).
