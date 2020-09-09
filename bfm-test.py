import scipy.io

BFM_path = 'models/BaselFaceModel_mod.mat'

model = scipy.io.loadmat(BFM_path)
print('loaded')