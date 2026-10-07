# Norn precompiler

Norn is the [Carla language](https://github.com/devlucasfs/carla) official precompiler. When u download Carla Norn is already downloaded too. 

You can see if the project is using Norn just looking the `target.eva` file.
```eva
@precompiler
' Carla ships with a built-in precompiler that handles the
' language's native preprocessing features. It is always enabled
' and cannot be disabled.
'
' Configure an additional precompiler below to extend the native
' preprocessing pipeline with extra capabilities. `norn` is the
' default extension, but you may replace it with any compatible
' implementation.
name: "norn"
```

as the Eva code says, you can change the precompiler to another one. Or maybe, do your own precompiler.

All you need to do a compatible precompiler, is respect 2 simple things:
- You need to receive the main file path in the 1th process argument
- You need to receive the `.e` (output) file path in the 2th process argument

### Simple, right?!
For example, when Carla runs Norn, Carla do:
```sh-session
norn "main absolute path" "output absolute path"
```

That means: That's all you need. 

You can use more arguments, if your project need. But, is recommended custom arguments be defined into the @precompiler namespace on `target.eva` file.
