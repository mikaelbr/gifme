module IpcRenderer = {
  type t;
  external ipcRenderer : t = "" [@@bs.scope "ipcRenderer"] [@@bs.module "electron"];
  external send : string => string => unit =
    "" [@@bs.scope "ipcRenderer"] [@@bs.module "electron"] [@@bs.val];
  external on : string => (unit => string => unit) => unit =
    "" [@@bs.scope "ipcRenderer"] [@@bs.module "electron"] [@@bs.val];
};
