%wort besteht mindestens aus aba
%        a     b
% ->z0 z0/z1  z0
%   z1   x    z2
%   z2  end   x
% * end end  end
 
%Aufgabe a)
zustand(z0).
zustand(z1).
zustand(z2).
zustand(end).
sigma(a).
sigma(b).
deltaf(z0,a,z1).
deltaf(z0,a,z0).
deltaf(z0,b,z0).
deltaf(z1,b,z2).
deltaf(z2,a,end).
deltaf(end,a,end).
deltaf(end,b,end).
start(z0).
end(end).

%Aufgabe b)

sigma_stern([]).
sigma_stern([X|Ws]):-sigma(X),sigma_stern(Ws).
lvonN(Ws):-start(S),delta_stern(S,Ws,Y),end(Y),sigma_stern(Ws).
delta(Zakt,A,Zneu):-deltaf(Zakt,A,Zneu).
delta_stern(Zakt,[],Zakt).
delta_stern(Zakt,[X|Ws],Zneu):-delta(Zakt,X,L),delta_stern(L,Ws,Zneu).