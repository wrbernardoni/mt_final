SET projectpath=C:\MTProject\mt_final

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\bg-en.train --test_file %projectpath%\data\bg-en.test --save_filename %projectpath%\NeuralOutput\bg_one_layer > %projectpath%\NeuralOutput\bg_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\bg-en.train --test_file %projectpath%\data\bg-en.test --save_filename %projectpath%\NeuralOutput\bg_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\bg_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\bg-en.train --test_file %projectpath%\data\bg-en.test --save_filename %projectpath%\NeuralOutput\bg_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\bg_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\bg-en.train --test_file %projectpath%\data\bg-en.test --save_filename %projectpath%\NeuralOutput\bg_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\bg_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\et-en.train --test_file %projectpath%\data\et-en.test --save_filename %projectpath%\NeuralOutput\et_one_layer > %projectpath%\NeuralOutput\et_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\et-en.train --test_file %projectpath%\data\et-en.test --save_filename %projectpath%\NeuralOutput\et_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\et_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\et-en.train --test_file %projectpath%\data\et-en.test --save_filename %projectpath%\NeuralOutput\et_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\et_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\et-en.train --test_file %projectpath%\data\et-en.test --save_filename %projectpath%\NeuralOutput\et_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\et_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\hu-en.train --test_file %projectpath%\data\hu-en.test --save_filename %projectpath%\NeuralOutput\hu_one_layer > %projectpath%\NeuralOutput\hu_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\hu-en.train --test_file %projectpath%\data\hu-en.test --save_filename %projectpath%\NeuralOutput\hu_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\hu_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\hu-en.train --test_file %projectpath%\data\hu-en.test --save_filename %projectpath%\NeuralOutput\hu_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\hu_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\hu-en.train --test_file %projectpath%\data\hu-en.test --save_filename %projectpath%\NeuralOutput\hu_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\hu_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\lv-en.train --test_file %projectpath%\data\lv-en.test --save_filename %projectpath%\NeuralOutput\lv_one_layer > %projectpath%\NeuralOutput\lv_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\lv-en.train --test_file %projectpath%\data\lv-en.test --save_filename %projectpath%\NeuralOutput\lv_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\lv_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\lv-en.train --test_file %projectpath%\data\lv-en.test --save_filename %projectpath%\NeuralOutput\lv_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\lv_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\lv-en.train --test_file %projectpath%\data\lv-en.test --save_filename %projectpath%\NeuralOutput\lv_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\lv_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\pl-en.train --test_file %projectpath%\data\pl-en.test --save_filename %projectpath%\NeuralOutput\pl_one_layer > %projectpath%\NeuralOutput\pl_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\pl-en.train --test_file %projectpath%\data\pl-en.test --save_filename %projectpath%\NeuralOutput\pl_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\pl_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\pl-en.train --test_file %projectpath%\data\pl-en.test --save_filename %projectpath%\NeuralOutput\pl_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\pl_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\pl-en.train --test_file %projectpath%\data\pl-en.test --save_filename %projectpath%\NeuralOutput\pl_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\pl_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\ro-en.train --test_file %projectpath%\data\ro-en.test --save_filename %projectpath%\NeuralOutput\ro_one_layer > %projectpath%\NeuralOutput\ro_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\ro-en.train --test_file %projectpath%\data\ro-en.test --save_filename %projectpath%\NeuralOutput\ro_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\ro_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\ro-en.train --test_file %projectpath%\data\ro-en.test --save_filename %projectpath%\NeuralOutput\ro_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\ro_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\ro-en.train --test_file %projectpath%\data\ro-en.test --save_filename %projectpath%\NeuralOutput\ro_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\ro_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sk-en.train --test_file %projectpath%\data\sk-en.test --save_filename %projectpath%\NeuralOutput\sk_one_layer > %projectpath%\NeuralOutput\sk_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sk-en.train --test_file %projectpath%\data\sk-en.test --save_filename %projectpath%\NeuralOutput\sk_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\sk_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sk-en.train --test_file %projectpath%\data\sk-en.test --save_filename %projectpath%\NeuralOutput\sk_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\sk_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sk-en.train --test_file %projectpath%\data\sk-en.test --save_filename %projectpath%\NeuralOutput\sk_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\sk_two_layers_64_l2.txt

python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sl-en.train --test_file %projectpath%\data\sl-en.test --save_filename %projectpath%\NeuralOutput\sl_one_layer > %projectpath%\NeuralOutput\sl_one_layer.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sl-en.train --test_file %projectpath%\data\sl-en.test --save_filename %projectpath%\NeuralOutput\sl_one_layer_l2 --l2_param 1 > %projectpath%\NeuralOutput\sl_one_layer_l2.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sl-en.train --test_file %projectpath%\data\sl-en.test --save_filename %projectpath%\NeuralOutput\sl_two_layers_64 --layers 64 > %projectpath%\NeuralOutput\sl_two_layers_64.txt
python neuralNet.py --num_epochs 100 --train_file %projectpath%\data\sl-en.train --test_file %projectpath%\data\sl-en.test --save_filename %projectpath%\NeuralOutput\sl_two_layers_64_l2 --layers 64 --l2_param 1 > %projectpath%\NeuralOutput\sl_two_layers_64_l2.txt
