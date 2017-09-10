type size = Js.t {. width : int, height : int};

type sizeOrBool =
  | Size size
  | Bool bool;

type userMediaOptions = Js.t {. audio : bool, video : sizeOrBool};

type mediaStream;

external getUserMedia : userMediaOptions => Js.Promise.t mediaStream =
  "" [@@bs.scope "navigator.mediaDevices"] [@@bs.val "getUserMedia"];
