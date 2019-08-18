%wort besteht aus gleich vielen bs wie as
%S->e
%S->aSb
%
%definition 9.2
zustand(z0).
zustand(z1).
zustand(z2).
 
sigma(a).
sigma(b).
 
kelleralphabet(a).
kelleralphabet(b).
kelleralphabet(c).
 
deltaf(z0,a,c,z0,[a,c]).
deltaf(z0,a,a,z0,[a,a]).
deltaf(z0,b,a,z1,[]).
deltaf(z1,b,a,z1,[]).
deltaf(z1,nix,c,z2,[]).
 
start(z0).
kellersymbol(c).
 
append([],Xs,Xs).
append([X|Xs] , Ys, [X|Rs]) :- append(Xs, Ys, Rs).
%abbildung 7.2 
sigma_stern([]).
sigma_stern([X|Ws]):-sigma(X),sigma_stern(Ws).
 
%definition 9.5, definition 9.2
lVonM(Ws):-start(S),kellersymbol(Boden),zustand(NewState),es_plus(S,Ws,[Boden],NewState,[],[]).

%definition 5.3/ 5.4
es_plus(S,Ws,Stacks,NeuS,WNs,NeuStacks):-es(S,Ws,Stacks,NeuS,WNs,NeuStacks).
es_plus(S,Ws,Stacks,NeuerZu,WNs,NeuStacks):-es(S, Ws, Stacks, NeuS, Wes, NeuStackess),es_plus(NeuS,Wes,NeuStackess,NeuerZu,WNs,NeuStacks).
%definition 9.4
es(S,[A|W],[Top|Stacks],NeuS,W,NeuStacks):-deltaf(S, A, Top, NeuS, AufStack),append(AufStack, Stacks, NeuStacks).
es(S,W,[Top|Stacks],NeuS,W,NeuStacks):-deltaf(S,nix,Top, NeuS, AufStack),append(AufStack, Stacks, NeuStacks).
