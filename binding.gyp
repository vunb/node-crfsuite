{
    "targets": [
        {
            "target_name": "crfsuite",
            "cflags_cc": ['-O3', '-Wall', '-pedantic', '-std=c++11', '-fexceptions', '-DUSE_SSE', '-DUSE_SSE2'],
            "conditions": [
                ["OS==\"mac\"", {
                    "xcode_settings": {
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                    }
                }]
            ],
            "sources": [
                "liblbfgs/lib/lbfgs.c",

                "crfsuite/lib/crf/src/crf1d_context.c",
                "crfsuite/lib/crf/src/crf1d_encode.c",
                "crfsuite/lib/crf/src/crf1d_feature.c",
                "crfsuite/lib/crf/src/crf1d_model.c",
                "crfsuite/lib/crf/src/crf1d_tag.c",
                "crfsuite/lib/crf/src/crfsuite.c",
                "crfsuite/lib/crf/src/crfsuite_train.c",
                "crfsuite/lib/crf/src/dataset.c",
                "crfsuite/lib/crf/src/dictionary.c",
                "crfsuite/lib/crf/src/holdout.c",
                "crfsuite/lib/crf/src/logging.c",
                "crfsuite/lib/crf/src/params.c",
                "crfsuite/lib/crf/src/quark.c",
                "crfsuite/lib/crf/src/rumavl.c",
                "crfsuite/lib/crf/src/train_arow.c",
                "crfsuite/lib/crf/src/train_averaged_perceptron.c",
                "crfsuite/lib/crf/src/train_l2sgd.c",
                "crfsuite/lib/crf/src/train_lbfgs.c",
                "crfsuite/lib/crf/src/train_passive_aggressive.c",

                "crfsuite/lib/cqdb/src/cqdb.c",
                "crfsuite/lib/cqdb/src/lookup3.c",

                "src/main.cc",
                "src/trainer_class.cc",
                "src/tagger_class.cc"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "liblbfgs/include",
                "crfsuite/include",
                "crfsuite/lib/cqdb/include"
            ]
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": ["<(module_name)"],
            "copies": [{
                "files": ["<(PRODUCT_DIR)/<(module_name).node"],
                "destination": "<(module_path)"
            }]
        }
    ]
}