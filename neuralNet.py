# Example run: python neuralNet.py --num_epochs 100 --train_file data/bg-en.train --test_file data/bg-en.test --save_filename \NeuralOutput\one_layer
# Example run with three extra layers: python neuralNet.py --num_epochs 100 --save_filename test --layers 64,32,16 --l2_param 1
import argparse
import matplotlib.pyplot as plt
from io import open

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.utils.data as util
from torch import optim

torch.manual_seed(0)


class Net(nn.Module):
    def __init__(self, layer_config):
        super(Net, self).__init__()
        layers = layer_config.split(',')
        self.layers = []
        if len(layers) == 1 and layers[0] == '':
            self.layers.append(nn.Linear(100, 100))
        else:
            size = 100
            for layer_size in layers:
                layer_size = int(layer_size)
                self.layers.append(nn.Linear(size, layer_size))
                self.layers.append(nn.ReLU())
                size = layer_size
            self.layers.append(nn.Linear(size, 100))

        self.model = nn.Sequential(*self.layers)

    def forward(self, x):
        return self.model(x)


def getTensor(filename):
    # LWords contains the ordering of the words
    l1Words = []
    l2Words = []

    # Opens the data file. It is formatted as
    # SrcWord TgtWord SrcWordVec Delim TgtWordVec
    rawdata = open(filename, 'r', encoding='utf-8')

    # LArrs contain lists of the 100 float vectors
    l1Arr = []
    l2Arr = []

    # Reads in input line by line to create LWords and creates
    # the LArrs by creating each LVec point by point
    for line in rawdata:
        first, second = line.split('|||')
        first_list = first.split()
        l1Words.append(first_list[0])
        l2Words.append(first_list[1])

        first_list = first_list[2:]
        second_list = second.split()

        l1Arr.append(list(map(lambda x: float(x), first_list)))
        l2Arr.append(list(map(lambda x: float(x), second_list)))

    l1M = torch.Tensor(l1Arr)
    l2M = torch.Tensor(l2Arr)

    # Error checking
    if len(l1Words) != len(l2Words):
        raise ValueError(filename + " is formatted incorrectly!")

    return (l1Words, l1M), (l2Words, l2M)


def train(model, optimizer, train_loader, epoch, loss_fn):
    model.train()
    train_loss = 0

    for batch_idx, (eng_data, bul_data) in enumerate(train_loader):
        optimizer.zero_grad()
        p = model(eng_data)
        loss, reg_loss = loss_fn(p, bul_data)

        reg_loss.backward()
        train_loss += loss.item()
        optimizer.step()

    train_loss /= len(train_loader)

    return train_loss


def test(model, test_loader, epoch, loss_fn):
    model.eval()
    with torch.no_grad():
        test_loss = 0

        for batch_idx, (eng_data, bul_data) in enumerate(test_loader):
            p = model(eng_data)
            loss, _ = loss_fn(p, bul_data)
            test_loss += loss.item()

        test_loss /= len(test_loader)

        return test_loss


def create_loss_fn(model, l2_param):
    def loss_fn2(y_pred, y_true):
        mse_loss = nn.MSELoss()
        loss = mse_loss(y_pred, y_true)
        reg_loss = loss
        if l2_param and model.training:
            for param in model.parameters():
                reg_loss = reg_loss + l2_param * mse_loss(param, torch.zeros(*param.size()))
        return loss, reg_loss
    return loss_fn2


def main():

    # Takes in command line arguments
    ap = argparse.ArgumentParser()
    ap.add_argument('--train_file', default='data/bg-en.train')
    ap.add_argument('--test_file', default='data/bg-en.test')
    ap.add_argument('--num_epochs', default='1')
    # Specify whether you want to use L2 regularization
    ap.add_argument('--l2_param', default=None, type=float)
    # Add additional linear layers with the following form "--layers 64,32" to add two layers with 64 output and 32 output
    ap.add_argument('--layers', default='')
    # Where the loss graph is saved (filename.png). To get exact numbers look at/pipe output
    ap.add_argument('--save_filename', required=True)

    args = ap.parse_args()

    # Returns the matrices P and Q as well as
    # a list of words corresponding to index
    # given the train and test files
    l1Train, l2Train = getTensor(args.train_file)
    l1Test, l2Test = getTensor(args.test_file)

    train_loader = util.DataLoader(util.TensorDataset(l1Train[1], l2Train[1]), batch_size=128, shuffle=True)
    test_loader = util.DataLoader(util.TensorDataset(l1Test[1], l2Test[1]), batch_size=512, shuffle=False)

    model = Net(args.layers)
    print(model)
    optimizer = optim.Adam(model.parameters(), lr=1e-3)

    loss_fn = create_loss_fn(model, args.l2_param)

    num_epochs = args.num_epochs

    train_losses = []
    test_losses = []
    for n in range(int(num_epochs)):
        train_loss = train(model, optimizer, train_loader, n, loss_fn)
        test_loss = test(model, test_loader, n, loss_fn)

        train_losses.append(train_loss * len(train_loader.dataset) * 100)
        test_losses.append(test_loss * len(test_loader.dataset) * 100)

    plt.plot(train_losses)
    plt.plot(test_losses)

    print(torch.Tensor(test_losses[-20:]).mean().item(), 
        torch.Tensor(test_losses[-20:]).std().item())
    plt.savefig(args.save_filename + ".png")


if __name__ == '__main__':
    main()
