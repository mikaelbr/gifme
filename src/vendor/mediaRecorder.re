type mediaRecorder;

type data = Js.t {. data : string};

external create_media_recorder : Navigator.mediaStream => mediaRecorder =
  "MediaRecorder" [@@bs.new];

external start : mediaRecorder => unit = "start" [@@bs.send];

external stop : mediaRecorder => unit = "stop" [@@bs.send];

external ondataavailable : mediaRecorder => (data => unit) => unit = "ondataavailable" [@@bs.set];

external onstop : mediaRecorder => (data => unit) => unit = "onstop" [@@bs.set];
