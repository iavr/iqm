% toy example to verify that mex and matlab versions of search_seq are consistent

% compile mex version
compile('../sub/search_seq', true)

% parameters/buffers
c = 9;
w = 7;
r = randperm(c);
A = zeros(c, 'uint32');
V = zeros(c, 'uint8');

% input data
P = uint32(floor(10*rand(c)));
I = uint32([randperm(c,w); randperm(c,w)])';
D = 50 * sort(rand(2,w,'single'), 2)';
T = uint32(50);
P, I = I', D = D'
I = I'; D = D';

% inversion
Z1 = zeros(1,c); Z1(I(:,1)') = 1:w;
Z2 = zeros(1,c); Z2(I(:,2)') = 1:w;

% mex version
C = search_seq(I-1, D, P, A, V, T) + 1;
D1 = D(:,1); D2 = D(:,2);
S = D1(Z1(C(:,1))) + D2(Z2(C(:,2)));

% matlab version
[mC,mS] = search_seq_mat(I-1, D, P, A, V, T);
mC = mC + 1;

% result/verification
C = C', mC = mC'
S = S', mS = mS'
assert(all(S(:) == mS(:)))
