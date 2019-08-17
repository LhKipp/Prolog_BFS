sigma(a).
sigma(b).

zustand(z0).
zustand(z1).

delta(z0,a,z1).
delta(z0,b,z1).
delta(z1,a,z0).
delta(z1,b,z0).

start_zustand(z0).
end_zustand(z0).

delta_stern(Zustandaktuell,[],Zustandaktuell).
delta_stern(Zustandaktuell,[X|Xs],Zustandneu):- zustand(Zneu),delta(Zustandaktuell,X,Zneu),delta_stern(Zneu,Xs,Zustandneu).

sigma_stern([]).
sigma_stern([X|Xs]) :- sigma(X), sigma_stern(Xs).

lvong11(Ws) :- start_zustand(S), end_zustand(E),sigma_stern(Ws),delta_stern(S,Ws,E).


























