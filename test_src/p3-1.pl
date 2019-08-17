% Datum: 23.05.2019

%leerer baum
baum(e).
%n stelliger baum
baum(n(X,Lb,Rb)):- baum(Lb),baum(Rb).

%prafix
prafix([],Xs).
prafix([X|Xs],[X|Ys]) :- prafix(Xs,Ys).

%Aufgabe 1.1 postfix(Xs,Ys) : Ys endet mit der Liste Xs.
%mit reverse und prafix
postfix1([X],[]).
postfix1(Xs,Ys):-reverse(Xs,Xs1),reverse(Ys,Ys1),prafix(Xs1,Ys1).
%postfix(Xs,Ys):-prafix(Xs,Ys).
%mit append
postfix2([X],[]).
postfix2(Xs,Ys):-append(_,Xs,Ys).

%Aufgabe 2 membertree(X,Xb) : Baum Xb enthaelt den Eintrag X.

membertree(X,n(X,Lb,Rb)):-baum(n(X,Lb,Rb)).
membertree(X,n(Y,Lb,Rb)):-baum(n(Y,Lb,Rb)),membertree(X,Lb).
membertree(X,n(Y,Lb,Rb)):-baum(n(Y,Lb,Rb)),membertree(X,Rb).

preorder(e,[]).
preorder(n(X,Lb,Rb),[X|Xs]):-baum(n(Root,Lb,Rb)),preorder(Lb,Lt),preorder(Rb,Rt),append(Lt,Rt,Xs).

postorder(e,[]).
postorder(n(X,Lb,Rb),Xs):-baum(n(Root,Lb,Rb)),postorder(Lb,Lt),postorder(Rb,Rt),append(Lt,Rt,W),append(W,[X],Xs).

roots(e,[]).
roots([n(X,Lb,Rb)|Xs],[X|Rests]):-roots(Xs,Rests).