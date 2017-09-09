module IpcRenderer = {
  type t;
  external send : t => string => string => unit = "" [@@bs.send];
  external on : t => string => (unit => string => unit) => unit = "" [@@bs.send];
};

external ipcRenderer : IpcRenderer.t = "" [@@bs.module "electron"];
