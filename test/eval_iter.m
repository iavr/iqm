global yael xio home
addpath('../config')
local
addpath(yael, xio);

data = fvecs_read('/home/user/sift/raw/sift_base.fvecs');
%data = load_double_array('/home/user/sift/raw/paris500k.bin', 'single');
data = single(data);
scores = zeros(10,1);
k_index = 1;
percentage = 0;
for k=1000:1000:1000
	for iter=5:5:20
		avgScore = 0;
		for inputID=0:0
			fprintf('Currently on k=%d#%d (%f%% completed)\n', k, inputID, (percentage/30*100))
			%centers=csvread(sprintf('./results/final_%d_%d.csv', k, i))';
			%centers=xload(sprintf('./results/final_%d_%d.f4', k, i));
			centers=xload(sprintf('./results/intermediate_%d_%d_%d.f4', k, inputID, iter)); 
			centers = single(centers);
			I = exp_assign(centers, data(:,1:100000));
			I2 = exp_assign(centers, data(:,100001:200000));
			I3 = exp_assign(centers, data(:,200001:300000));
			I4 = exp_assign(centers, data(:,300001:400000));
			I5 = exp_assign(centers, data(:,400001:500000));%size(data,2)));
			I6 = exp_assign(centers, data(:,500001:600000));
			I7 = exp_assign(centers, data(:,600001:700000));
			I8 = exp_assign(centers, data(:,700001:800000));
			I9 = exp_assign(centers, data(:,800001:900000));
			I10 = exp_assign(centers, data(:,900001:size(data,2)));
			ass = [I I2 I3 I4 I5 I6 I7 I8 I9 I10];
			clear('I', 'I2', 'I3', 'I4', 'I5', 'I6', 'I7', 'I8', 'I9', 'I10')
			scr = exp_computeDistortion(centers, data, ass);
			avgScore = scr + avgScore;
			clear('ass');
			percentage = percentage + 1;
		end
		scores(k_index) = avgScore;% / 3;
		k_index = k_index + 1;
	end
end
%csvwrite(sprintf('./results/scores.csv', i), scores);
