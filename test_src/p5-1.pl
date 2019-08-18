%program_code.push_back("zustand(readSigma).");
%program_code.push_back("zustand(dele).");
%program_code.push_back("sigma(a).");
%program_code.push_back("sigma(b).");
%program_code.push_back("kellerSym(raute).");
%program_code.push_back("kellerSym(a).");
%program_code.push_back("delta(readSigma, nix, raute, readSigma ,nil).");
%program_code.push_back("delta(readSigma, a, raute, readSigma ,list(a,list(raute,nil))).");
%program_code.push_back("delta(readSigma, a, a, readSigma ,list(a,list(a,nil))).");
%program_code.push_back("delta(readSigma, nix, a, dele, list(a,nil)).");
%program_code.push_back("delta(readSigma, b, a, dele ,nil).");
%program_code.push_back("delta(dele, b, a, dele ,nil).");
%program_code.push_back("delta(dele, nix, raute, dele ,nil).");
%program_code.push_back("startZ(readSigma).");
%program_code.push_back("startKeller(raute).");
%program_code.push_back("sigma_stern(nil).");
%program_code.push_back("sigma_stern(list(W, Rs) :- sigma(W), sigma_stern(Rs).");
%program_code.push_back("lappend(nil, Xs, Xs).");
%program_code.push_back("lappend(list(X, Rs), Xs, list(X, Ns)) :- lappend(Rs, Xs, Ns).");
%program_code.push_back("es(Zustand, list(W, WRs) , list(G,GRs), ZNext, WRs, Stack_neu) :- delta(Zustand, W, G, ZNext, WriteBack), lappend(Wri%teBack, GRs, Stack_neu).");
%program_code.push_back("es(Zustand, W, list(G, GRs), ZNext, W, Stack_neu) :- delta(Zustand, nix, G, ZNext, WriteBack), lappend(WriteBack, GRs%, Stack_neu).");
%program_code.push_back("es_plus(Z0, W, G, Z1, WNeu, GNeu) :- es(Z0, W, G, Z1 , WNeu, GNeu).");
%program_code.push_back("es_plus(Z0, W, G, ZNeu, WRs, Stack_neu) :- es(Z0, W, G, ZNext, WRest, Stack), es_plus(ZNext, WRest, Stack, ZNeu, WRs,% Stack_neu).");
%program_code.push_back("lVonM(Ws) :- startZ(Z0), zustand(EndZ), startKeller(K) , es_plus(Z0, Ws, list(K,nil), EndZ, nil, nil).");

zustand(readSigma).
zustand(dele).
sigma(a).
sigma(b).
kellerSym(raute).
kellerSym(a).
delta(readSigma, nix, raute, readSigma ,nil).
delta(readSigma, a, raute, readSigma ,list(a,list(raute,nil))).
delta(readSigma, a, a, readSigma ,list(a,list(a,nil))).
delta(readSigma, nix, a, dele, list(a,nil)).
delta(readSigma, b, a, dele ,nil).
delta(dele, b, a, dele ,nil).
delta(dele, nix, raute, dele ,nil).
startZ(readSigma).
startKeller(raute).
sigma_stern(nil).
sigma_stern(list(W, Rs)) :- sigma(W), sigma_stern(Rs).
lappend(nil, Xs, Xs).
lappend(list(X, Rs), Xs, list(X, Ns)) :- lappend(Rs, Xs, Ns).
es(Zustand, list(W, WRs) , list(G,GRs), ZNext, WRs, Stack_neu) :- delta(Zustand, W, G, ZNext, WriteBack), lappend(WriteBack, GRs, Stack_neu).
es(Zustand, W, list(G, GRs), ZNext, W, Stack_neu) :- delta(Zustand, nix, G, ZNext, WriteBack), lappend(WriteBack, GRs, Stack_neu).
es_plus(Z0, W, G, Z1, WNeu, GNeu) :- es(Z0, W, G, Z1 , WNeu, GNeu).
es_plus(Z0, W, G, ZNeu, WRs, Stack_neu) :- es(Z0, W, G, ZNext, WRest, Stack), es_plus(ZNext, WRest, Stack, ZNeu, WRs, Stack_neu).
lVonM(Ws) :- startZ(Z0), zustand(EndZ), startKeller(K) , es_plus(Z0, Ws, list(K,nil), EndZ, nil, nil).
