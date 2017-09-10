external createObjectURL : Blob.t => string = "URL.createObjectURL" [@@bs.val];

let component = ReasonReact.statelessComponent "LoopVideo";

let make ::chunk ::onComplete ::onCancel _children => {
  ...component,
  render: fun _self => {
    let url = createObjectURL chunk;
    <div className="capture__doneRecording">
      <video autoPlay=Js.true_ loop=Js.true_ src=url />
      <div className="confirmBox">
        <button onClick=(fun _ => onCancel ())> (ReasonReact.stringToElement {js|⟲|js}) </button>
        <button onClick=(fun _ => onComplete url)>
          (ReasonReact.stringToElement {js|✔|js})
        </button>
      </div>
    </div>
  }
};
