% pda von abb 9.8 (M-PalinBin)

zustand(z0).
sigma(0).
sigma(1).
gamma(0).
gamma(1).
gamma(s).

delta(z0, 0, 0, z0, []).
delta(z0, 1, 1, z0, []).
delta(z0, nix, s, z0, []).
delta(z0, nix, s, z0, [0]).
delta(z0, nix, s, z0, [1]).
delta(z0, nix, s, z0, [0, s, 0]).
delta(z0, nix, s, z0, [1, s, 1]).

startz(z0).
kellerboden(s).


% aufgabe b
% einzelschrittrelation (9.4)
es(Zakt, [A | Ws], [Kpop | Kbleibt], Zneu, Ws, Kapp) :-
    delta(Zakt, A, Kpop, Zneu, Kpush),
    zustand(Zakt), zustand(Zneu), sigma(A), sigma_stern(Ws), gamma(Kpop),
    append(Kpush, Kbleibt, Kapp),
    gamma_stern(Kpush), gamma_stern(Kbleibt).

es(Zakt, Ws, [Kpop | Kbleibt], Zneu, Ws, Kapp) :-
    delta(Zakt, nix, Kpop, Zneu, Kpush),
    zustand(Zakt), zustand(Zneu), sigma_stern(Ws), gamma(Kpop),
    append(Kpush, Kbleibt, Kapp),
    gamma_stern(Kpush), gamma_stern(Kbleibt).

% transitiver abschluss - mehrere Schritte
es_plus(Zakt, W, K, Zneu, Wneu, Kneu)
	:- es(Zakt, W, K, Zneu, Wneu, Kneu).

es_plus(Zakt, Wakts, Kakts, Zneu, Wneus, Kneus) :-
    es(Zakt, Wakts, Kakts, Zuebergang, Wuebergangs, Kuebergangs),
    es_plus(Zuebergang, Wuebergangs, Kuebergangs, Zneu, Wneus, Kneus).

% menge der wörter über sigma
sigma_stern([]).
sigma_stern([A | Ws]) :- sigma(A), sigma_stern(Ws).

% wörter im keller
gamma_stern([]).
gamma_stern([A|Ws]) :- gamma(A), gamma_stern(Ws).

% von pda erkannte sprache (9.5)
lvonM(Ws) :- sigma_stern(Ws), zustand(Zneu), es_plus(Zakt, Ws, [K], Zneu, [], []),
    startz(Zakt), kellerboden(K).
