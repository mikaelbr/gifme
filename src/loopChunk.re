external createObjectURL : Blob.t => string = "URL.createObjectURL" [@@bs.val];

let component = ReasonReact.statelessComponent "LoopVideo";

let make ::chunk ::onComplete _children => {
  ...component,
  render: fun _self =>
    <div className="capture__doneRecording">
      <video autoPlay=Js.true_ loop=Js.true_ src=(createObjectURL chunk) />
      <h1> (ReasonReact.stringToElement "Is Recording") </h1>
      <button onClick=(fun _ => onComplete ())>
        (ReasonReact.stringToElement "Send recording")
      </button>
    </div>
};
