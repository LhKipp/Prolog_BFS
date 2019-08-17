% NEA von bsp 8.12 (end01)
vari(vS).
vari(vA).
vari(vend).
sigma(0).
sigma(1).
start(vS).
delta(vA, 1, vend). % ende und anfang müssen nach oben, da diese bevorzugt genommen werden müssen
delta(vS, 0, vA).
delta(vS, 0, vS).
delta(vS, 1, vS).
end(vend).

sigma_stern(_X).
sigma_stern([X|Xs]) :- sigma(X), sigma_stern(Xs).

delta_stern(Zakt, [], Zakt).
delta_stern(Zakt, [X|Xs], Zneu) :-
    delta(Zakt, X, Zuebergang),
    delta_stern(Zuebergang, Xs, Zneu).

lvonN(Ws) :- start(Z0), end(E), delta_stern(Z0, Ws, E), sigma_stern(Ws). % sigma stern muss ans ende, sonst läuft man in die unendlichkeit des sigmas
