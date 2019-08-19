sigma(a).
sigma(b).

zustand(za).
zustand(zb).

delta(za,a,zb).
delta(za,b,zb).
delta(zb,a,za).
delta(zb,b,za).

start_zustand(za).
end_zustand(za).

delta_stern(Zustandaktuell,[],Zustandaktuell).
delta_stern(Zustandaktuell,[X|Xs],Zustandneu):- zustand(Zneu),delta(Zustandaktuell,X,Zneu),delta_stern(Zneu,Xs,Zustandneu).

sigma_stern([]).
sigma_stern([X|Xs]) :- sigma(X), sigma_stern(Xs).

lvongbb(Ws) :- start_zustand(S), end_zustand(E),sigma_stern(Ws),delta_stern(S,Ws,E).


























