% pda von abb 9.8 (M-Palin ab)

zustand(za).
sigma(a).
sigma(b).
gamma(a).
gamma(b).
gamma(s).

delta(za, a, a, za, []).
delta(za, b, b, za, []).
delta(za, nix, s, za, []).
delta(za, nix, s, za, [a]).
delta(za, nix, s, za, [b]).
delta(za, nix, s, za, [a, s, a]).
delta(za, nix, s, za, [b, s, b]).

startz(za).
kellerboden(s).

appendl([], Xs,Xs).
appendl([X|Xs], Ys,[X | Rs]) :- appendl(Xs, Ys , Rs).

% aufgabe b
% einzelschrittrelation (9.4)
es(Zakt, [A | Ws], [Kpop | Kbleibt], Zneu, Ws, Kapp) :-
    delta(Zakt, A, Kpop, Zneu, Kpush),
    appendl(Kpush, Kbleibt, Kapp).

es(Zakt, Ws, [Kpop | Kbleibt], Zneu, Ws, Kapp) :-
    delta(Zakt, nix, Kpop, Zneu, Kpush),
    appendl(Kpush, Kbleibt, Kapp).

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
lvonM(Ws) :- kellerboden(K),startz(Zakt),  zustand(Zneu),es_plus(Zakt, Ws, [K], Zneu, [], []).
