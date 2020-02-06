% Aufgabe 1
% praefix(Xs, Ys) : Ys beginnt mit Xs
praefix(Xs, Ys) :- append(Xs, A_Es, Ys).

% postfix(Xs,Ys) : Ys endet mit der Liste Xs.
postfix1(Xs, Ys) :- reverse(Xs, Xsr), praefix(Xsr, Xsprae), reverse(Xsprae, Ys).

postfix2(Xs, Ys) :- append(A_Es, Xs, Ys).


% Aufgabe 2
% baum: e ist der leere baum
binbaum(e).
% wenn Ab und Bb Teilbäume sind, dann kann der nächsthöhere Baum diese beiden enthalten
binbaum(n(X, Ab, Bb)) :- binbaum(Ab), binbaum(Bb).

% member(X,Xb) : Baum Xb enthält den Eintrag X.
member(X, n(X, Lb, Rb)) :- binbaum(Lb), binbaum(Rb).
member(X, n(Y, Lb, Rb)) :- member(X, Lb), binbaum(Lb), binbaum(Rb).
member(X, n(Y, Lb, Rb)) :- member(X, Rb), binbaum(Lb), binbaum(Rb).


% Aufgabe 3
% präorder(Xb,Ys) : Ys ist die Liste der Knotenbeschriftungen des Binärbaumes Xb in Präorder.
praeorder(e, []).
praeorder(n(Root, Lb, Rb), Ys) :- praeorder(Lb, Ls), praeorder(Rb, Rs), append([Root | Ls], Rs, Ys).

% postorder(Xb,Ys) : Ys ist die Liste der Knotenbeschriftungen des Binärbaumes Xb in Postorder.
postorder(e, []).
% die appends müssen auf die andere seite, je nachdem welche query man stellt (welcher baum erzeugt Xs vs welche postorder kommt durch baum Xb) 
postorder(n(Root, Lb, Rb), Ys) :-  postorder(Lb, Ls), postorder(Rb, Rs), append(Rs, [Root], Rroots), append(Ls, Rroots, Ys).

% roots(Xbs,Ys) : Xbs ist eine Liste von Binärbäumen (geschachtelte Induktion).
% Die Liste Ys ist die Liste der Wurzelbeschriftungen der Binärbäume in Xbs
% in der richtigen Reihenfolge. Beachten Sie, dass ein leerer Binärbaum keine
% Wurzelbeschriftung hat und diese somit auch nicht aufgeführt wird.
roots([], []).
roots([e | Xbs], Ys) :- roots(Xbs, Ys).
roots([n(X, Lb, Rb) | Xbs], [X | Ys]) :- roots(Xbs, Ys).







