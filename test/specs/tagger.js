'use strict';
const test = require('tape');
const path = require('path');
const crfsuite = require('../..');
const tagger = new crfsuite.Tagger();

const input = [['Chào', 'bias', 'word.lower=chào', 'word.isupper=False', 'word.istitle=True', 'word.isdigit=False', 'BOS', '2_word.in_dictionary=1', '-2_word:BOS', '-3_word:BOS', '3_word.in_dictionary=0', 'BOS', '+1:word.lower=mừng', '+2:word.lower=các'], ['mừng', 'bias', 'word.lower=mừng', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=chào', '-1:word.istitle=True', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=1', '-3_word:BOS', '3_word.in_dictionary=0', 'BOS', '+1:word.lower=các', '+2:word.lower=bạn'], ['các', 'bias', 'word.lower=các', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=mừng', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=chào', '+1:word.lower=bạn', '+2:word.lower=trẻ'], ['bạn', 'bias', 'word.lower=bạn', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=các', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=mừng', '+1:word.lower=trẻ', '+2:word.lower=tới'], ['trẻ', 'bias', 'word.lower=trẻ', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=bạn', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=các', '+1:word.lower=tới', '+2:word.lower=thành'], ['tới', 'bias', 'word.lower=tới', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=trẻ', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=bạn', '+1:word.lower=thành', '+2:word.lower=phố'], ['thành', 'bias', 'word.lower=thành', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=tới', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=1', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=trẻ', '+1:word.lower=phố', '+2:word.lower=hà'], ['phố', 'bias', 'word.lower=phố', 'word.isupper=False', 'word.istitle=False', 'word.isdigit=False', '-1:word.lower=thành', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=1', '-3_word.in_dictionary=0', '3_word.in_dictionary=0', '-2:word.lower=tới', '+1:word.lower=hà', '+2:word.lower=nội'], ['Hà', 'bias', 'word.lower=hà', 'word.isupper=False', 'word.istitle=True', 'word.isdigit=False', '-1:word.lower=phố', '-1:word.istitle=False', '-1:word.isupper=False', '2_word.in_dictionary=0', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word:EOS', '-2:word.lower=thành', '+1:word.lower=nội', 'EOS'], ['Nội', 'bias', 'word.lower=nội', 'word.isupper=False', 'word.istitle=True', 'word.isdigit=False', '-1:word.lower=hà', '-1:word.istitle=True', '-1:word.isupper=False', '2_word:EOS', '-2_word.in_dictionary=0', '-3_word.in_dictionary=0', '3_word:EOS', '-2:word.lower=phố', 'EOS', 'EOS']]

test('crfsuite tagger', function (t) {
  t.plan(2)

  let model_path = path.resolve(path.join(__dirname, '../models/wordsent_crf_v1.model'))
  console.log('File model path: ' + model_path)
  let is_opened = tagger.open(model_path)

  t.ok(is_opened, 'File model is opened')

  let tags = tagger.tag(input)
  console.log('Tags: ', tags)

  t.deepEqual(tags, ['B_W', 'I_W', 'B_W', 'B_W', 'B_W', 'B_W', 'B_W', 'I_W', 'B_W', 'I_W'], `['Chào mừng', 'các', 'bạn', 'trẻ', 'tới', 'thành phố', 'Hà Nội']`)
})
