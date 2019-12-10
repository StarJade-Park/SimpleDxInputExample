# SimpleDxInputExample

Simple DirectInput example

## How To Use
1. Create
```c++
  std::unique_ptr<GInput> pInputInstance = std::unique_ptr<GInput>(new GInput(HWND, int, int));
```

2. Update
```c++
  GInput::Update();
```

2. Release(Choose one of the methods here)
```c++
  GInput::Release();
```

```c++
  pInputInstance->ShutDown();
```

```c++
  // pInputInstance release
```
