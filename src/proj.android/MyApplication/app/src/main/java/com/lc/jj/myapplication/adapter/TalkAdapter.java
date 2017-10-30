package com.lc.jj.myapplication.adapter;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.lc.jj.myapplication.R;
import com.lc.jj.myapplication.bean.TalkBean;

import java.util.List;

import butterknife.BindView;
import butterknife.ButterKnife;
import simple.config.LConnection;

/**
 * Created by JJ on 2017/10/16.
 */

public class TalkAdapter extends RecyclerView.Adapter<TalkAdapter.VHTalk> {

    List<TalkBean> list;

    public TalkAdapter(List<TalkBean> list) {
        this.list = list;
    }

    @Override
    public VHTalk onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.adapter_talk, parent, false);
        return new VHTalk(view);
    }

    @Override
    public void onBindViewHolder(VHTalk holder, int position) {
        TalkBean data = list.get(position);

        StringBuilder sb = new StringBuilder();
        sb.append(data.type == LConnection.TYPE_P2P ? "[P2P]" : "[TEAM]").append("  |").append(data.from)
                .append("| 对 |").append(data.to).append("| 说 {").append(data.content).append("}")
                .append("{").append(data.ext).append("}");
        holder.text.setText(sb.toString());
    }

    @Override
    public int getItemCount() {
        return list == null ? 0 : list.size();
    }

    public static class VHTalk extends RecyclerView.ViewHolder {

        @BindView(R.id.tv_talk)
        TextView text;

        public VHTalk(View itemView) {
            super(itemView);
            ButterKnife.bind(this, itemView);
        }
    }
}
