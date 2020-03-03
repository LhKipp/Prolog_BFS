% NEA von bsp 8.12 (end01)
vari(vS).
vari(vA).
vari(vend).
sigma(b).
sigma(a).
start(vS).
delta(vA, a, vend). 
% ende und anfang müssen nach oben, da diese bevorzugt genommen werden müssen
delta(vS, b, vA).
delta(vS, b, vS).
delta(vS, a, vS).
end(vend).

sigma_stern(A_X).
sigma_stern([X|Xs]) :- sigma(X), sigma_stern(Xs).

delta_stern(Zakt, [], Zakt).
delta_stern(Zakt, [X|Xs], Zneu) :- delta(Zakt, X, Zuebergang), delta_stern(Zuebergang, Xs, Zneu).
lvonN(Ws) :- start(Za), end(E), delta_stern(Za, Ws, E), sigma_stern(Ws). 
% sigma stern muss ans ende, sonst läuft man in die unendlichkeit des sigmas
