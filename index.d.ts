export declare class Tagger {
  tag(xseq: Array<string[]>): { probability: number; result: string[] };
  open(model_filename: string): boolean;
}

export interface Options {
  [key: string]: string;
}

export interface TrainerCallback {
  (str: string): void;
}

export declare class Trainer {
  append(xseq: Array<string[]>, yseq: string[]): void;
  train(model_filename: string): void;
  set_params(options: Options): void;
  set_callback(callback: TrainerCallback): void;
}
