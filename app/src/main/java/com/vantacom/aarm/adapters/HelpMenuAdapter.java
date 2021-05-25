package com.vantacom.aarm.adapters;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.vantacom.aarm.R;

import java.util.ArrayList;

public class HelpMenuAdapter extends RecyclerView.Adapter<HelpMenuAdapter.ViewHolder> {
    private ArrayList<String> actions;
    private ArrayList<String> descriptions;

    public HelpMenuAdapter(ArrayList<String> actionsSet, ArrayList<String> descriptionsSet) {
        actions = actionsSet;
        descriptions = descriptionsSet;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View view = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.help_menu_line, viewGroup, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder viewHolder, final int position) {
        viewHolder.getTextAction().setText(actions.get(position));
        viewHolder.getTextDescription().setText(descriptions.get(position));
    }

    @Override
    public int getItemCount() {
        return Math.min(descriptions.size(), actions.size());
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        private TextView textAction, textDescription;

        public ViewHolder(View view) {
            super(view);
            textAction = view.findViewById(R.id.action);
            textDescription = view.findViewById(R.id.description);
        }

        public TextView getTextAction() {
            return textAction;
        }

        public TextView getTextDescription() {
            return textDescription;
        }
    }
}
