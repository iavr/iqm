	data = fvecs_read('/home/user/sift/raw/sift_base.fvecs');
	%data = load_double_array('/home/user/sift/raw/paris500k.bin', 'single');
	data = single(data);
	size(data)
	data(1:10, 1:10)
			scores = zeros(5,1);
	for k=1000:1000:10000
		avgTime = 0;
		avgScore = 0;
		for i=0:4
			i
			totalAvgScore = [];
			totalAvgTime = [];
			jjj = 1;
				centers=csvread(sprintf('./data/initial_centroids_%d_%d.csv', k, i))';
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
			%	ass = [I I2 I3 I4 I5];
				clear('I', 'I2', 'I3', 'I4', 'I5', 'I6', 'I7', 'I8', 'I9', 'I10')
			%	clear('I', 'I2', 'I3', 'I4', 'I5')
				scr = exp_computeDistortion(centers, data, ass);
				avgScore = scr + avgScore;
				scores(jjj) = scr;
scores
				jjj = jjj + 1;
				clear('ass');
			end
			csvwrite(sprintf('./iter_%d/scores.csv', i), scores);
		end
		totalAvgScore = [totalAvgScore (avgScore / 5)];
